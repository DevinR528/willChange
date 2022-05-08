
# vim: set tabstop=4 smarttab

CC ?= gcc

buildtype ?= release

#CPPFLAGS += -Wfatal-errors
CPPFLAGS += -I .

CXXFLAGS += -Wall -Wextra -Werror
CXXFLAGS += -std=c++2a

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CXXFLAGS += -O2
CXXFLAGS += -flto
else
CPPFLAGS += -D DEBUGGING

CXXFLAGS += -g
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-unused-function
endif

on_error ?= do_nothing

ifeq ($(on_error), do_nothing)
ON_ERROR =
else ifeq ($(on_error), open_editor)
ON_ERROR += || ($$EDITOR $<; false)
else
$(error "invalid on_error option!");
endif

buildprefix ?= build/cc-$(CC)/$(buildtype)

LDFLAGS += -static

LDLIBS += -lstdc++

all: $(buildprefix)/zade

.PRECIOUS: %/

%/:
	@ mkdir -p $@

build/srclist.mk: | build/
	find -name '*.cpp' | sed 's/^/srcs += /' > $@

include build/srclist.mk

ARGS += -i examples/helloworld.zade

run: $(buildprefix)/zade
	$< $(ARGS)

valrun: $(buildprefix)/zade
	valgrind --gen-suppressions=yes --suppressions=./stl-val.supp --track-origins=yes --keep-debuginfo=yes $< $(ARGS)

$(buildprefix)/zade: $(patsubst %.cpp,$(buildprefix)/%.o,$(srcs))
	@ echo "linking $@"
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(buildprefix)/%.o $(buildprefix)/%.d: %.cpp | $(buildprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -MD -o $(buildprefix)/$*.o $(ON_ERROR)

format:
	find -name '*.cpp' -o -name '*.hpp' | xargs -d \\n clang-format -i --verbose

clean:
	rm -rf build zade

distclean: clean
	# Let me keep my vscode project level config
	for l in $$(cat .gitignore); do if [ $$l != ".vscode" ]; then rm -rf $$l; fi done

ifneq "$(MAKECMDGOALS)" "fetch"
include $(patsubst %.cpp,$(buildprefix)/%.d,$(srcs))
endif







