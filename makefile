
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
	find -name '*.cpp' | sed 's/^/srcs += /' > $@

include build/srclist.mk

ARGS += -i test_in/foobar.zd

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

# for make-check this is what clang-tidy depends on to check all files of project
$(buildprefix)/%.cmpdb_entry: $(srcs)
	@ echo "	{" > $@
	@ echo "		\"command\": \"cc $(CPPFLAGS) $(CXXFLAGS) -c $*.cpp\"," >> $@
	@ echo "		\"directory\": \"$(CURDIR)\"," >> $@
	@ echo "		\"file\": \"$*.cpp\"" >> $@
	@ echo "	}," >> $@

COMPDB_ENTRIES = $(patsubst %.cpp, $(buildprefix)/%.cmpdb_entry, $(srcs))

compile_commands.json: $(COMPDB_ENTRIES)
	@ echo "[" > $@.tmp
	@ cat $^ >> $@.tmp
	@ sed '$$d' < $@.tmp > $@
	@ echo "	}" >> $@
	@ echo "]" >> $@
	@ rm $@.tmp

check: compile_commands.json
	find -name '*.cpp' -o -name '*.hpp' | xargs -d \\n clang-tidy  --warnings-as-errors=*

clean:
	rm -rf build zade compile_commands.json

distclean: clean
	# Let me keep my vscode project level config
	for l in $$(cat .gitignore); do if [ $$l != ".vscode" ]; then rm -rf $$l; fi done

ifneq "$(MAKECMDGOALS)" "fetch"
include $(patsubst %.cpp,$(buildprefix)/%.d,$(srcs))
endif







