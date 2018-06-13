# user configuration point
TARGET := MyHello.so
PASS := myhello
CFLAGS :=
CXXFLAGS := -Wall -Wextra -Wpedantic
LDFLAGS :=
OPTFLAGS := -time-passes
VERSION := 4.0
# default option
RM ?= rm -f

# llvm/clang tools
ifdef VERSION
suffix := -$(VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)
cc := clang$(suffix)
opt := opt$(suffix)
# llvm flags
llvm-cxxflags != $(config) --cxxflags
llvm-ldflags != $(config) --ldflags
# adapt flags to clang options
llvm-cxxflags += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
llvm-cxxflags := $(subst $(from),$(to),$(llvm-cxxflags))

# local options
cflags := -c -S -emit-llvm
cxxflags := -c
ldflags := -shared
optflags := -analyze -load $(CURDIR)/$(TARGET) -$(PASS)
# add user configuration
cflags += $(CFLAGS)
cxxflags += $(CXXFLAGS)
ldflags += $(LDFLAGS)
optflags += $(OPTFLAGS)
# add llvm flags
cxxflags += $(llvm-cxxflags)
ldflags += $(llvm-ldflags)

# collect sub-targets
srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
testsrcs := $(wildcard test/*.c)
testcases := $(testsrcs:%.c=%)

all: $(TARGET)

$(TARGET): $(objs)
	$(cxx) $(ldflags) -o $@ $^

%.o: %.cpp
	$(cxx) $(cxxflags) -o $@ $<

%.ll: %.c
	$(cc) $(cflags) -o $@ $<

test: $(testcases)

$(testcases): test/%: test/%.ll $(TARGET)
	$(opt) $(optflags) $<

distclean: clean clean/$(TARGET)

clean: clean/objs clean/tests

clean/$(TARGET):
	$(RM) $(TARGET)

clean/objs:
	$(RM) $(objs)

clean/tests:
	$(RM) $(wildcard test/*.ll)

.PHONY: all test $(testcases)
.PHONY: distclean clean clean/$(TARGET) clean/objs clean/tests
