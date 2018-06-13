CXXFLAGS != llvm-config --cxxflags
LDFLAGS != llvm-config --ldflags

LDFLAGS += -shared

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
