#!/usr/bin/make -f

# target
PASS := stacksafe
TARGET := $(PASS).so
path := $(CURDIR)/$(TARGET)
compile-commands := compile_commands.json

# llvm
LLVM_SUFFIX ?=
LLVM_CONFIG ?= llvm-config$(LLVM_SUFFIX)
llvm-cxxflags != $(LLVM_CONFIG) --cxxflags
llvm-ldflags != $(LLVM_CONFIG) --ldflags
llvm-filter := -std=% -fuse-ld=% -Wl,% -O% -g% -DNDEBUG
llvm-cxxflags := $(filter-out $(llvm-filter),$(llvm-cxxflags))
CXX := clang++
LD := ld.lld
cxx := clang++
ld := ld.lld

# flags
CXXFLAGS += -std=c++17 -fPIC -pedantic -Wall -Wextra
LDFLAGS += -shared
CXXFLAGS += $(llvm-cxxflags)
LDFLAGS += $(llvm-ldflags)
release-flags := -O2 -DNDEBUG
debug-flags := -O0 -g3

srcdir := src
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:%.cpp=%.o)
jsons := $(srcs:%.cpp=%.json)

export CXX CXXFLAGS

.SUFFIXES:

.PHONY: default
default:

$(TARGET): $(objs)
	$(LD) $(LDFLAGS) $^ -o $@

.INTERMEDIATE: $(jsons)
$(objs) $(jsons):
	make -C $(@D) $(@F)

.PHONY: release debug
release debug: $(TARGET)
release: CXXFLAGS += $(release-flags)
debug: CXXFLAGS += $(debug-flags)

# googletest
libgtest := libgtest.a
gtestdir := googletest/googletest
gtestincludes := -isystem $(gtestdir)/include -I$(gtestdir)
gtestflags := -std=c++11 -Wall -Wextra -pedantic -O0 -g3 -pthread
gtestsrc := $(gtestdir)/src/gtest-all.cc
gtestobj := $(gtestsrc:%.cc=%.o)

.INTERMEDIATE: $(gtestobj)
$(gtestobj): $(gtestsrc)
	$(cxx) $(gtestflags) $(gtestincludes) -o $@ -c $<
$(libgtest): $(gtestobj)
	$(AR) -r $@ $^

# unittest
unitdir := unittest
unitincludes := $(gtestincludes) -I$(srcdir)
unitcxxflags := $(unitincludes) $(cxxflags)
unitldflags := $(llvm-ldflags) $(libgtest) -lLLVM
unitsrcs := $(wildcard $(unitdir)/*.cpp)
unitlsps := $(unitsrcs:%.cpp=%.json)
units := $(unitsrcs:%.cpp=%)

$(units): %: %.cpp
	$(cxx) $(unitcxxflags) -o $@ $< $(unitldflags)
unittest: $(units)

.INTERMEDIATE: $(unitlsps)
$(unitlsps): %.json: %.cpp
	@$(cxx) $(unitflags) -MJ $@ -fsyntax-only $<

# compile commands
$(compile-commands): $(jsons)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $^ >$@

# analysis
opt := opt$(LLVM_SUFFIX)
optflags := -analyze -load=$(path) -$(PASS)
#optflags += -time-passes

irdir := ir
test-prefix := test
run-prefix := run
irs := $(wildcard $(irdir)/*.ll)
tests := $(irs:$(irdir)/%.ll=$(test-prefix)/%)
runs := $(irs:$(irdir)/%.ll=$(run-prefix)/%)

.PHONY: $(tests)
$(tests): optflags += -debug
$(tests): $(test-prefix)/%: $(run-prefix)/%

.PHONY: $(runs)
$(runs): $(run-prefix)/%: $(irdir)/%.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends   ----

# clean
cleans := $(addprefix clean/,units)

.PHONY: clean $(cleans) distclean
clean: $(cleans)
	make -C src clean
$(cleans): clean/%:
	$(RM) $($*)
distclean: clean
	@$(RM) $(compile-commands) $(TARGET)
