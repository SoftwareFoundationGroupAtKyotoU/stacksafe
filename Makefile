#!/usr/bin/make -f

# target
PASS := stacksafe
TARGET := $(PASS).so
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

.PHONY: all
all: release

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
cc := clang
opt := opt$(LLVM_SUFFIX)
CFLAGS += -S -emit-llvm
path := $(CURDIR)/$(TARGET)
optflags := -analyze -load=$(path) -$(PASS)
#optflags += -time-passes

irdir := ir
emit-prefix := emit
test-prefix := test
run-prefix := run
irsrcs != find $(CURDIR) -type f -name '*.c'
irobjs := $(wildcard $(irdir)/*.ll)
emits := $(irsrcs:$(CURDIR)/%.c=$(emit-prefix)/%)
tests := $(irobjs:$(irdir)/%.ll=$(test-prefix)/%)
runs := $(irobjs:$(irdir)/%.ll=$(run-prefix)/%)

.PHONY: $(emits)
$(emits): $(emit-prefix)/%: %.c
	@$(cc) $(CFLAGS) -o $(irdir)/$(notdir $*).ll -c $<

.PHONY: $(tests)
$(tests): optflags += -debug
$(tests): $(test-prefix)/%: $(run-prefix)/%

.PHONY: $(runs)
$(runs): $(run-prefix)/%: $(irdir)/%.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends ----

# clean
cleans := $(addprefix clean/,units)

.PHONY: clean $(cleans) distclean
clean: $(cleans)
	make -C src clean
$(cleans): clean/%:
	$(RM) $($*)
distclean: clean
	@$(RM) $(compile-commands) $(TARGET)
