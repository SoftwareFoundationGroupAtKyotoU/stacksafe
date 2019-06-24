#!/usr/bin/make -f

libg := libgtest.a
gdir := $(CURDIR)/googletest/googletest
gsrc := $(gdir)/src/gtest-all.cc
gobj := $(gsrc:%.cc=%.o)

srcdir := src
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:$(srcdir)/%.cpp=%)
jsons := $(srcs:%.cpp=%.json)

incs := -isystem $(gdir)/include -I$(gdir)
CXXFLAGS += $(incs) -pthread -O0 -g3

$(gobj): CXXFLAGS += -std=c++11
$(objs) $(jsons): CXXFLAGS += -std=c++17 -I$(TOPDIR)/src
$(objs): LDFLAGS += -lLLVM

.INTERMEDIATE: $(gobj)
$(gobj): $(gsrc)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(libg): $(gobj)
	$(AR) -r $@ $^

$(objs): %: $(srcdir)/%.cpp $(libg)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	./$(objs)

$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -fsyntax-only $*.cpp -MJ $@

.PHONY: clean
clean:
	$(RM) $(objs)
