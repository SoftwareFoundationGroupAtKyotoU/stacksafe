#!/usr/bin/make -f

CXX := clang++
CXXFLAGS := -std=c++17 -fPIC

cxxflags-filter := -std=%
CXXFLAGS += $(filter-out $(cxxflags-filter),$(LLVM_CXXFLAGS))

srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)

$(objs): %.o: %.cpp
	$(info OBJS: $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(objs)
