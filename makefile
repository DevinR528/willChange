
CC ?= gcc
CXX ?= g++
#CXX ?= clang++

buildtype ?= release

# CPPFLAGS += -I .
# CPPFLAGS += -D _GNU_SOURCE

#CPPFLAGS += -D FMT_HEADER_ONLY
CPPFLAGS += -I ./vendor/fmt-8.1.1/include

CXXFLAGS += -Wall -Werror
CXXFLAGS += -std=c++2a

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CXXFLAGS += -O2
CXXFLAGS += -flto
else
CPPFLAGS += -D DEBUGGING
endif

buildprefix ?= build/$(buildtype)

CXXFLAGS += -g
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-unused-function

SRCS += main.cc
SRCS += parse.cc

OBJS = $(patsubst %.cc,$(buildprefix)/%.o,$(SRCS))

LDFLAGS += -static

LDLIBS += -L ./vendor/fmt-8.1.1

LDLIBS += -lfmt
LDLIBS += -lstdc++

all: $(buildprefix)/zade

.PRECIOUS: %/

%/:
	@ mkdir -p $@

ARGS += -i foobar.zd

run: $(buildprefix)/zade
	$< $(ARGS)

valrun: $(buildprefix)/zade
	valgrind $< $(ARGS)

$(buildprefix)/zade: $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(buildprefix)/%.o: %.cc | $(buildprefix)/%/
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

https//%: | https/
	@ mkdir -p "$(@D)"
	wget --no-use-server-timestamps -O "$@" https://$*

format:
	clang-format -i --verbose $(SRCS)

fetch: https//github.com/fmtlib/fmt/releases/download/8.1.1/fmt-8.1.1.zip | vendor/
	unzip -o -qq $< -d $|
	cd $|/fmt-8.1.1; cmake .
	make -C $|/fmt-8.1.1 fmt/fast -j8

.depend: $(SRCS)
	$(CXX) $(CPPFLAGS) -MM $^ > .tmp-depend
	@ mv .tmp-depend $@

clean:
	rm -rf build zade

distclean: clean
	for l in $$(cat .gitignore); do rm -rf $$l; done

ifneq "$(MAKECMDGOALS)" "fetch"
include .depend
endif









