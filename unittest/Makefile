#!/usr/bin/make -f

libg := libgtest.a
gdir := googletest/googletest
gsrc := $(gdir)/src/gtest-all.cc
gobj := $(gsrc:%.cc=%.o)

srcs := $(wildcard *.cpp)
tests := $(srcs:%.cpp=%)
jsons := $(srcs:%.cpp=%.json)

incs := -isystem $(gdir)/include -I$(gdir)
CXXFLAGS += $(incs) -pthread -O0 -g3

$(gobj): CXXFLAGS += -std=c++11
$(tests) $(jsons): CXXFLAGS += -std=c++17 -I../src
$(tests): LDFLAGS += -lLLVM

.INTERMEDIATE: $(gobj)
$(gobj): $(gsrc)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(libg): $(gobj)
	$(AR) -r $@ $^

$(tests): %: %.cpp $(libg)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -fsyntax-only $*.cpp -MJ $@

.PHONY: clean
clean:
	$(RM) $(tests)
