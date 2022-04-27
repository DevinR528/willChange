CC=gcc
CXX=clang++
RM=rm -f

buildtype = debug

# CPPFLAGS += -I .
# CPPFLAGS += -D _GNU_SOURCE

CXXFLAGS += -Wall -Werror
CXXFLAGS += -std=c++2a

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CXXFLAGS += -O2
CXXFLAGS += -flto
else
CPPFLAGS += -D DEBUGGING
endif

CXXFLAGS += -g
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-unused-function

SRCS=main.cc parse.cc
OBJS=$(subst %.cc,build/$(buildtype)/%.o,$(SRCS))

LDLIBS += -L./include/fmt-8.1.1/libfmt.so.8.1.1

run: zade
	./zade -i foobar.zd

all: zade

zade: $(OBJS)
	$(CXX) $(CXXFLAGS) -o zade $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

fetch:
	mkdir ./include/
	cd ./include && \
	wget https://github.com/fmtlib/fmt/releases/download/8.1.1/fmt-8.1.1.zip && \
	unzip ./fmt-8.1.1.zip && \
	cd ./fmt-8.1.1 && \
	cmake -DBUILD_SHARED_LIBS=TRUE && \
	make fmt/fast && \
    cd .. \
    rm ./fmt-8.1.1.zip

clean:
	$(RM) $(OBJS)
	$(RM) includes/

distclean: clean
	$(RM) *~ .depend

include .depend
