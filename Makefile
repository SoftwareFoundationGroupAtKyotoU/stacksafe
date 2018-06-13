LLVM_CONFIG ?= llvm-config
CC := clang
CXX ?= clang++
CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?= -shared

ifdef VERSION
LLVM_CONFIG := llvm-config-$(VERSION)
CC := clang-$(VERSION)
CXX := clang++-$(VERSION)
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

TARGET := MyHello.so
PASSNAME := myhello
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
GENERATED := $(TARGET) $(OBJS)
TESTSRCS := $(wildcard test/*.c)
TESTCASES := $(basename $(TESTSRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.ll: %.c
	$(CC) -c -S -emit-llvm -o $@ $<

$(TESTCASES): test/%: test/%.ll
	opt -load $(TARGET) -$(PASSNAME) -f $< >/dev/null

clean:
	$(RM) -f $(GENERATED)

.PHONY: all clean $(TESTCASES)
