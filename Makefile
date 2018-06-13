LLVM_CONFIG ?= llvm-config
CXX ?= clang++
CC := clang
OPT := opt
RM ?= rm -f
OUTPUT_OPTION ?= -o $@

CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?= -shared
OPTFLAGS ?= -analyze -time-passes

CFLAGS += -c -S -emit-llvm $(OUTPUT_OPTION)
CXXFLAGS += -c $(OUTPUT_OPTION)

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

TARGET := MyHello.so
PASSNAME := myhello
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
GENERATED := $(TARGET) $(OBJS)
TESTSRCS := $(wildcard test/*.c)
TESTCASES := $(basename $(TESTSRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $<

%.ll: %.c
	$(CC) $(CFLAGS) $<

$(TESTCASES): test/%: test/%.ll
	$(OPT) $(OPTFLAGS) -load $(TARGET) -$(PASSNAME) -f $< >/dev/null

clean:
	$(RM) $(GENERATED)

.PHONY: all clean $(TESTCASES)
