#!/usr/bin/make -f

libgtest := libgtest.a
gtestdir := googletest/googletest
gtestincludes := -isystem $(gtestdir)/include -I$(gtestdir)
gtestflags := -std=c++11 -Wall -Wextra -pedantic -O0 -g3 -pthread
gtestflags += $(gtestincludes)

unitcxxflags := $(gtestincludes) -I../src -pthread
unitldflags := -lLLVM

gtestsrc := $(gtestdir)/src/gtest-all.cc
gtestobj := $(gtestsrc:%.cc=%.o)
srcs := $(wildcard *.cpp)
tests := $(srcs:%.cpp=%)
jsons := $(srcs:%.cpp=%.json)

.INTERMEDIATE: $(gtestobj)
$(gtestobj): $(gtestsrc)
	$(CXX) $(gtestflags) -o $@ -c $<
$(libgtest): $(gtestobj)
	$(AR) -r $@ $^

$(tests) $(jsons): CXXFLAGS += $(unitcxxflags)
$(tests): LDFLAGS += $(unitldflags)

$(tests): %: %.cpp $(libgtest)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -MJ $@ -fsyntax-only $*.cpp

.PHONY: clean
clean:
	$(RM) $(tests)
