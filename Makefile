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

depend-filter  =   sed -e 's,^$(notdir $*)\.o:,$*.o $@:,'
depend-filter += | sed -e 's, /usr/[^ ]*, ,g' -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): %.d: %.cpp
	$(info DEPS: $@)
	@$(cxx) $(cxxflags) -MM $< | $(depend-filter) >$@

.INTERMEDIATE: $(lsps)
$(lsps): %.json: %.cpp
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
	@$(RM) $(objs) $(deps)

.PHONY: distclean
distclean:
	@$(RM) $(compile-commands) $(TARGET)
