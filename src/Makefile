#!/usr/bin/make -f

CXXFLAGS += -std=c++17 -fPIC

srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
deps := $(srcs:%.cpp=deps/%.d)
jsons := $(srcs:%.cpp=%.json)

.SUFFIXES:

.PHONY: default
default:

$(objs): %.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $@

depend-filter  =   sed -e 's,^$(notdir $*.o):,$*.o $@:,'
depend-filter += | sed -e 's, /usr/[^ ]*,,g' -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): deps/%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< | $(depend-filter) >$@

-include $(deps)

$(jsons): %.json: %.cpp
	$(CXX) $(CXXFLAGS) -fsyntax-only $< -MJ $@

.PHONY: clean
clean:
	$(RM) $(wildcard *.o) $(wildcard *.d)
