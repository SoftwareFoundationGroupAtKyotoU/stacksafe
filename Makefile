LLVM_CONFIG := llvm-config
CXX := clang++
CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?= -shared

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

TARGET := LLVMHello.so
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
GENERATED := $(TARGET) $(OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	$(RM) -f $(GENERATED)

.PHONY: all clean
