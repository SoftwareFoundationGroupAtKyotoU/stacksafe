#!/usr/bin/make -f

srcs := $(wildcard *.cpp)
tests := $(srcs:%.cpp=%)
jsons := $(srcs:%.cpp=%.json)

$(tests): %: %.cpp
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) -o $@

$(jsons): %.json:
	$(CXX) $(CXXFLAGS) -MJ $@ -fsyntax-only $*.cpp

.PHONY: clean
clean:
	$(RM) $(tests)
