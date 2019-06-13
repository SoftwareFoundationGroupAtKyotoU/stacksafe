#!/usr/bin/make -f

CXX := clang++
LD := ld.lld

PASS := stacksafe
TARGET := $(PASS).so
TOPDIR := $(CURDIR)
compile-commands := compile_commands.json

LLVM_SUFFIX ?=
LLVM_CONFIG ?= llvm-config$(LLVM_SUFFIX)
llvm-cxxflags != $(LLVM_CONFIG) --cxxflags
llvm-ldflags != $(LLVM_CONFIG) --ldflags
llvm-filter := -std=% -fuse-ld=% -Wl,% -O% -g% -DNDEBUG
llvm-cxxflags := $(filter-out $(llvm-filter),$(llvm-cxxflags))

CXXFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += $(llvm-cxxflags) -Wno-unknown-warning-option
LDFLAGS += $(llvm-ldflags)
release-flags := -O2 -DNDEBUG
debug-flags := -O0 -g3

export CXX CXXFLAGS LD LDFLAGS
export LLVM_SUFFIX LLVM_CONFIG
export PASS TARGET TOPDIR

srcdir := src
testdir := gtest
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:%.cpp=%.o)
deps := $(srcs:%.cpp=%.d)
testsrcs := $(wildcard $(testdir)/*.cpp)
testobjs := $(testsrcs:%.cpp=%)
jsonsrcs := $(srcs) $(testsrcs)
jsons := $(jsonsrcs:%.cpp=%.json)

.SUFFIXES:
.PHONY: default
default:

.PHONY: release debug
release: CXXFLAGS += $(release-flags)
debug: CXXFLAGS += $(debug-flags)
$(TARGET): LDFLAGS += -shared

release debug: $(TARGET)
$(TARGET): $(objs)
	$(LD) $(LDFLAGS) $^ -o $@

.INTERMEDIATE: $(jsons)
.PHONY: $(compile-commands)
$(compile-commands): $(jsons)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $^ >$@

$(objs) $(testobjs) $(jsons):
	make -C $(@D) $(@F)

depend-filter  =   sed -e 's,^$(notdir $*.o):,$*.o $@:,'
depend-filter += | sed -e 's, /usr/[^ ]*,,g' -e 's,^ \+,,'
depend-filter += | sed -e 's,\\$$,,' | tr -d '\n'
$(deps): %.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< | $(depend-filter) >$@
$(testobjs): %: %.cpp

-include $(deps)

# clean
clean-prefix := clean
cleanfiles := $(addprefix $(clean-prefix)/,$(compile-commands) $(TARGET))
cleandirs := $(addprefix $(clean-prefix)/,$(srcdir) $(testdir))

.PHONY: $(cleanfiles) $(cleandirs) clean distclean
$(cleandirs):
	make -C $(@F) $(@D)
$(cleanfiles):
	$(RM) $(@F)
clean: $(cleandirs)
distclean: clean $(cleanfiles)
