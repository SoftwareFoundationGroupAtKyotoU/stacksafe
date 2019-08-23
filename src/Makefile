#!/usr/bin/make -f

CXXFLAGS += -std=c++17 -fPIC

srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
jsons := $(srcs:%.cpp=%.json)

.SUFFIXES:

.PHONY: default
default:

$(objs): %.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $@

$(jsons): %.json: %.cpp
	$(CXX) $(CXXFLAGS) -fsyntax-only $< -MJ $@

.PHONY: clean
clean:
	$(RM) $(wildcard *.o) $(wildcard *.d)
