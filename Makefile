LLVM_CONFIG ?= llvm-config
CXX ?= clang++
CC := clang
RM ?= rm -f
OUTPUT_OPTION ?= -o $@

CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?= -shared

CFLAGS += -c -S -emit-llvm
CXXFLAGS += -c

ifdef VERSION
LLVM_CONFIG := llvm-config-$(VERSION)
CXX := clang++-$(VERSION)
CC := clang-$(VERSION)
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
	$(CXX) $(LDFLAGS) $(OUTPUT_OPTION) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OUTPUT_OPTION) $<

%.ll: %.c
	$(CC) $(CFLAGS) $(OUTPUT_OPTION) $<

$(TESTCASES): test/%: test/%.ll
	opt -load $(TARGET) -$(PASSNAME) -f $< >/dev/null

clean:
	$(RM) $(GENERATED)

.PHONY: all clean $(TESTCASES)
