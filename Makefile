#!/usr/bin/make -f

# target
PASS := stacksafe
TARGET := $(PASS).so
TARGET_PATH := $(CURDIR)/$(TARGET)
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

# flags
CXXFLAGS += -pedantic -Wall -Wextra
CXXFLAGS += $(llvm-cxxflags)
LDFLAGS += $(llvm-ldflags)
release-flags := -O2 -DNDEBUG
debug-flags := -O0 -g3

srcdir := src
srcs := $(wildcard $(srcdir)/*.cpp)
objs := $(srcs:%.cpp=%.o)
jsons := $(srcs:%.cpp=%.json)

export PASS TARGET_PATH LLVM_SUFFIX CXX CXXFLAGS LDFLAGS

.SUFFIXES:

.PHONY: default
default:

$(TARGET): LDFLAGS += -shared
$(TARGET): $(objs)
	$(LD) $(LDFLAGS) $^ -o $@

.INTERMEDIATE: $(jsons)
$(objs) $(jsons):
	make -C $(@D) $(@F)

.PHONY: release debug
release debug: $(TARGET)
release: CXXFLAGS += $(release-flags)
debug: CXXFLAGS += $(debug-flags)

# unittest
unitdir := unittest
unitsrcs := $(wildcard $(unitdir)/*.cpp)
unitjsons := $(unitsrcs:%.cpp=%.json)
units := $(unitsrcs:%.cpp=%)

.INTERMEDIATE: $(unitjsons)
$(units) $(unitjsons):
	make -C $(@D) $(@F)

# compile commands
$(compile-commands): $(jsons) $(unitjsons)
	sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' $^ >$@

# analysis
.PHONY: test run
test run:
	make -C ir $@

# clean
clean-prefix := clean
cleanfiles := $(addprefix $(clean-prefix)/,$(compile-commands) $(TARGET))
cleandirs := $(addprefix $(clean-prefix)/,$(srcdir) $(unitdir))

.PHONY: $(cleanfiles) $(cleandirs) clean distclean
$(cleandirs):
	make -C $(@F) $(@D)
$(cleanfiles):
	$(RM) $(@F)
clean: $(cleandirs)
distclean: clean $(cleanfiles)
