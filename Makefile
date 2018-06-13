CXXFLAGS != llvm-config --cxxflags
LDFLAGS != llvm-config --ldflags

LDFLAGS += -shared

TARGET := LLVMHello.so
SRCS := $(wildcard *.cpp)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
