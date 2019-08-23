#!/usr/bin/make -f

libg := libgtest.a
gdir := $(CURDIR)/googletest/googletest
gsrc := $(gdir)/src/gtest-all.cc
gobj := $(gsrc:%.cc=%.o)

srcs := $(wildcard src/*.cpp)
tests := $(srcs:src/%.cpp=%)
targets := $(tests:%=%.out)
jsons := $(tests:%=%.json)

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

$(targets): %.out: src/%.cpp $(libg)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(jsons): %.json: src/%.cpp
	$(CXX) $(CXXFLAGS) -fsyntax-only $< -MJ $@

.PHONY: $(tests)
$(tests): %: %.out
	./$<

.PHONY: clean distclean
clean:
	$(RM) $(targets) $(jsons)
distclean: clean
	$(RM) $(libg)
