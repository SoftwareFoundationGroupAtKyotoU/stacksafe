#!/usr/bin/make -f

CXX := clang++
CXXFLAGS := -std=c++17 -fPIC

cxxflags-filter := -std=%
CXXFLAGS += $(filter-out $(cxxflags-filter),$(LLVM_CXXFLAGS))

srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
deps := $(srcs:%.cpp=%.d)
jsons := $(srcs:%.cpp=%.json)

$(objs): %.o: %.cpp
	$(info OBJS: $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

depend-filter  =   sed -e 's,^$*.o:,$*.o $@:,'
depend-filter += | sed -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): %.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< | $(depend-filter) >$@

.INTERMEDIATE: $(jsons)
$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -fsyntax-only $*.cpp -MJ $@

clean:
	$(RM) $(objs) $(deps)

-include $(deps)
