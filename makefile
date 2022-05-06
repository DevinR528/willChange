
CC ?= gcc

buildtype ?= release

CPPFLAGS += -Wfatal-errors
CPPFLAGS += -I .

CXXFLAGS += -Wall -Werror
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
	find -name '*.cc' | sed 's/^/srcs += /' > $@

include build/srclist.mk

ARGS += -i test_in/foobar.zd

run: $(buildprefix)/zade
	$< $(ARGS)

valrun: $(buildprefix)/zade
	valgrind --gen-suppressions=yes --suppressions=./stl-val.supp --track-origins=yes --keep-debuginfo=yes $< $(ARGS)

$(buildprefix)/zade: $(patsubst %.cc,$(buildprefix)/%.o,$(srcs))
	@ echo "linking $@"
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(buildprefix)/%.o $(buildprefix)/%.d: %.cc | $(buildprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -MD -o $(buildprefix)/$*.o $(ON_ERROR)

format:
	find -name '*.cc' -o -name '*.hpp' | xargs -d \\n clang-format -i --verbose

clean:
	rm -rf build zade

distclean: clean
	for l in $$(cat .gitignore); do rm -rf $$l; done

ifneq "$(MAKECMDGOALS)" "fetch"
include $(patsubst %.cc,$(buildprefix)/%.d,$(srcs))
endif







