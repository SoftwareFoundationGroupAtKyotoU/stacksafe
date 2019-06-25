#!/usr/bin/make -f

libg := libgtest.a
gdir := $(CURDIR)/googletest/googletest
gsrc := $(gdir)/src/gtest-all.cc
gobj := $(gsrc:%.cc=%.o)

srcdir := src
srcs := $(wildcard $(srcdir)/*.cpp)
targets := $(srcs:$(srcdir)/%.cpp=%)
jsons := $(srcs:%.cpp=%.json)

incs := -isystem $(gdir)/include -I$(gdir)
CXXFLAGS += $(incs) -pthread -O0 -g3

$(gobj): CXXFLAGS += -std=c++11
$(targets) $(jsons): CXXFLAGS += -std=c++17 -I$(TOPDIR)/src
$(targets): LDFLAGS += -lLLVM

.INTERMEDIATE: $(gobj)
$(gobj): $(gsrc)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(libg): $(gobj)
	$(AR) -r $@ $^

$(targets): %: $(srcdir)/%.cpp $(libg)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -fsyntax-only $*.cpp -MJ $@

.PHONY: clean
clean:
	$(RM) $(targets)
