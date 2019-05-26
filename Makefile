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
llvm-includedir != $(LLVM_CONFIG) --includedir
cxxflags-out := -std=% -fuse-ld=% -O% -g% -DNDEBUG -Wl,%
cxxflags := $(CXXFLAGS) $(filter-out $(cxxflags-out),$(llvm-cxxflags))
ldflags := $(LDFLAGS) $(llvm-ldflags)

srcdir := src
objdir := obj
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:$(srcdir)/%.cpp=$(objdir)/%.o)
deps := $(objs:%.o=%.d)
lsps := $(objs:%.o=%.json)

.SUFFIXES:

.PHONY: all
all: release

$(TARGET): $(objs)
	$(info TARGET: $@)
	@$(ld) $(ldflags) $(OUTPUT_OPTION) $^

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
	@$(cxx) $(cxxflags) $(OUTPUT_OPTION) -c $<

depend-filter  =   sed -e 's,$*\.o,$(@D)/$*.o $@,g'
depend-filter += | sed -e 's, /usr/[^ ]*, ,g' -e 's,^ \+,,g'
depend-filter += | sed -e 's,\\$$,,g' | tr -d '\n'
$(deps): $(objdir)/%.d: $(srcdir)/%.cpp
	$(info DEPS: $@)
	@$(cxx) $(cxxflags) -MM $< | $(depend-filter) >$@

$(lsps): $(objdir)/%.json: $(srcdir)/%.cpp
	@$(cxx) $(cxxflags) -MJ $@ -fsyntax-only $<
$(compile-commands): $(lsps)
	@sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $^ >$@

-include $(deps)

# test
cc := clang
opt := opt$(LLVM_SUFFIX)
cflags := -S -emit-llvm $(CFLAGS)
path := $(CURDIR)/$(TARGET)
optflags := -analyze -load=$(path) -$(PASS)
#optflags += -time-passes

testdir := test
irsrcs := $(wildcard $(testdir)/*.c)
irobjs := $(wildcard $(testdir)/*.ll)
tests := $(irobjs:%.ll=%)
runs := $(tests:$(testdir)/%=run/%)

$(irsrcs:%.c=%.ll): %.ll: %.c
	$(cc) $(cflags) $(OUTPUT_OPTION) -c $<

.PHONY: $(tests)
$(tests): optflags += -debug
$(tests): $(testdir)/%: $(testdir)/%.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends ----

.PHONY: $(runs)
$(runs): run/%: $(testdir)/%.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends ----

.PHONY: clean
clean:
	@$(RM) $(objs) $(deps) $(lsps)

.PHONY: distclean
distclean:
	@$(RM) -r $(objdir) $(compile-commands) $(TARGET)
