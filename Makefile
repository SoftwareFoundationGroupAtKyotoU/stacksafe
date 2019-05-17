#!/usr/bin/make -f

PASS := stacksafe
TARGET := $(PASS).so
LLVM_SUFFIX ?= -7
config := llvm-config$(LLVM_SUFFIX)
cxx := clang++$(LLVM_SUFFIX)
# options
CXXFLAGS ?= -c -std=c++17 -Wno-unknown-warning-option
LDFLAGS ?= -shared -fuse-ld=lld
release-flags := -O2 -DNDEBUG
debug-flags := -O0 -g3

# flags
llvm-cxxflags != $(config) --cxxflags
llvm-ldflags != $(config) --ldflags
llvm-includedir != $(config) --includedir
cxxflags-out := -std=% -fuse-ld=% -O% -g% -DNDEBUG -Wl,%
cxxflags := $(CXXFLAGS) $(filter-out $(cxxflags-out),$(llvm-cxxflags))
ldflags := $(LDFLAGS) $(llvm-ldflags)

srcdir := src
objdir := obj
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:$(srcdir)/%.cpp=$(objdir)/%.o)
deps := $(objs:%.o=%.d)

.SUFFIXES:

.PHONY: all
all: release

$(TARGET): $(objs)
	$(info TARGET: $@)
	@$(cxx) $(ldflags) $(OUTPUT_OPTION) $^

.PHONY: release
release: cxxflags += $(release-flags)
release: $(TARGET)

.PHONY: debug
debug: cxxflags += $(debug-flags)
debug: $(TARGET)

.SECONDEXPANSION:
$(objs) $(deps): | $$(@D)
$(objdir):
	@mkdir $@

$(objs): $(objdir)/%.o: $(srcdir)/%.cpp
	$(info OBJS: $@)
	@$(cxx) $(cxxflags) $(OUTPUT_OPTION) $<

depend-output = $(cxx) -I$(llvm-includedir) -MM $<
depend-output += | sed -e 's,$*\.o,$(@D)/$*.o $@,g'
depend-output += | sed -e 's, /usr/[^ ]*, ,g' -e 's,^ \+,,g'
depend-output += | sed -e 's,\\$$,,g' | tr -d '\n'
depend-output += | tee $@ >/dev/null
$(deps): $(objdir)/%.d: $(srcdir)/%.cpp
	$(info DEPS: $@)
	@$(depend-output)

-include $(deps)

# test
cc := clang$(LLVM_SUFFIX)
opt := opt$(LLVM_SUFFIX)
cflags := -c -S -emit-llvm $(CFLAGS)
path := $(CURDIR)/$(TARGET)
optflags := -analyze -load=$(path) -$(PASS)
#optflags += -time-passes

testdir := test
irsrcs := $(wildcard $(testdir)/*.c)
irobjs := $(wildcard $(testdir)/*.ll)
tests := $(irobjs:%.ll=%)

$(irsrcs:%.ll=%.c): %.ll: %.c
	$(cc) $(cflags) $(OUTPUT_OPTION) $<

.PHONY: $(tests)
$(tests): $(testdir)/%: $(testdir)/%.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends ----
	@echo

.PHONY: clean
clean:
	@$(RM) $(objs) $(deps)

.PHONY: distclean
distclean:
	@$(RM) -r $(objdir) $(TARGET)
