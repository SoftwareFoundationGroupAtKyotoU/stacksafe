LLVM_CONFIG ?= llvm-config
CXX ?= clang++
CC := clang
OPT := opt
RM ?= rm -f
OUTPUT_OPTION ?= -o $@
TARGET ?= MyHello.so
PASS ?= myhello

CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?=
OPTFLAGS ?= -time-passes

ifdef VERSION
LLVM_CONFIG := llvm-config-$(VERSION)
CXX := clang++-$(VERSION)
CC := clang-$(VERSION)
OPT := opt-$(VERSION)
endif

LLVM_CXXFLAGS != $(LLVM_CONFIG) --cxxflags
LLVM_LDFLAGS != $(LLVM_CONFIG) --ldflags

findclang := $(findstring clang,$(CXX))
ifeq (clang,$(findclang))
LLVM_CXXFLAGS += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
LLVM_CXXFLAGS := $(subst $(from),$(to),$(LLVM_CXXFLAGS))
endif

CXXFLAGS += $(LLVM_CXXFLAGS)
LDFLAGS += $(LLVM_LDFLAGS)

CFLAGS += -c -S -emit-llvm
CXXFLAGS += -c
LDFLAGS += -shared
OPTFLAGS += -analyze -load $(CURDIR)/$(TARGET) -$(PASS)

CFLAGS += $(OUTPUT_OPTION)
CXXFLAGS += $(OUTPUT_OPTION)
LDFLAGS += $(OUTPUT_OPTION)

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
TESTSRCS := $(wildcard test/*.c)
TESTCASES := $(TESTSRCS:%.c=%)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $<

%.ll: %.c
	$(CC) $(CFLAGS) $<

test: $(TESTCASES)

$(TESTCASES): test/%: test/%.ll $(TARGET)
	$(OPT) $(OPTFLAGS) $<

clean: clean/objs clean/tests

distclean: clean clean/$(TARGET)

clean/$(TARGET):
	$(RM) $(TARGET)

clean/objs:
	$(RM) $(OBJS)

clean/tests:
	$(RM) $(wildcard test/*.ll)

.PHONY: all test $(TESTCASES)
.PHONY: clean clean/$(TARGET) clean/objs clean/tests
