#!/usr/bin/make -f

srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
deps := $(srcs:%.cpp=%.d)
jsons := $(srcs:%.cpp=%.json)

.SUFFIXES:

.PHONY: default
default:

$(objs): %.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $@

depend-filter  =   sed -e 's,^$*.o:,$*.o $@:,'
depend-filter += | sed -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): %.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< | $(depend-filter) >$@

.INTERMEDIATE: $(jsons)
$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -fsyntax-only $*.cpp -MJ $@

.PHONY: clean
clean:
	$(RM) $(objs) $(deps)

-include $(deps)
