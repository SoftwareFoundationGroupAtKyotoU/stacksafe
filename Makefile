#!/usr/bin/make -f

PASS := stacksafe
TARGET := $(PASS).so
LLVM_SUFFIX ?=
LLVM_CONFIG ?= llvm-config$(LLVM_SUFFIX)
cxx := clang++
ld := ld.lld
# options
CXXFLAGS += -std=c++17 -fPIC
LDFLAGS += -shared
release-flags := -O2 -DNDEBUG
debug-flags := -O0 -g3
compile-commands := compile_commands.json

# flags
llvm-cxxflags != $(LLVM_CONFIG) --cxxflags
llvm-ldflags != $(LLVM_CONFIG) --ldflags
cxxflags-out := -std=% -fuse-ld=% -O% -g% -DNDEBUG -Wl,%
cxxflags := $(CXXFLAGS) $(filter-out $(cxxflags-out),$(llvm-cxxflags))
ldflags := $(LDFLAGS) $(llvm-ldflags)

srcdir := src
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:%.cpp=%.o)
deps := $(srcs:%.cpp=%.d)
lsps := $(srcs:%.cpp=%.json)

.SUFFIXES:

.PHONY: all
all: release

$(TARGET): $(objs)
	$(info TARGET: $@)
	@$(ld) $(ldflags) -o $@ $^

.PHONY: release debug
release debug: $(TARGET)
release: cxxflags += $(release-flags)
debug: cxxflags += $(debug-flags)

$(objs): %.o: %.cpp
	$(info OBJS: $@)
	@$(cxx) $(cxxflags) -o $@ -c $<

.INTERMEDIATE: $(lsps)
$(lsps): %.json: %.cpp
	@$(cxx) $(cxxflags) -MJ $@ -fsyntax-only $<

depend-filter  =   sed -e 's,^$(notdir $*)\.o:,$*.o $@:,'
depend-filter += | sed -e 's, /usr/[^ ]*, ,g' -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): %.d: %.cpp
	$(info DEPS: $@)
	@$(cxx) $(cxxflags) -MM $< | $(depend-filter) >$@

-include $(deps)

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
$(compile-commands): $(lsps) $(unitlsps)
	@sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $^ >$@

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

.PHONY: clean
clean:
	@$(RM) $(objs) $(deps)

.PHONY: distclean
distclean:
	@$(RM) $(compile-commands) $(TARGET)
