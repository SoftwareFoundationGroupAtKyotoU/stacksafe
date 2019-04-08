#!/usr/bin/make -f

TARGET := stacksafe.so
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
dependflags := -I$(llvm-includedir) -MM
dependscript = sed -e 's,$(notdir $*).o,$*.o $*.d,g'

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
	$(info RELEASE:)

.PHONY: debug
debug: cxxflags += $(debug-flags)
debug: $(TARGET)
	$(info DEBUG:)

$(objs): $(objdir)/%.o: $(srcdir)/%.cpp
	$(info OBJS: $@)
	@mkdir -p $(objdir)
	@$(cxx) $(cxxflags) $(OUTPUT_OPTION) $<

$(deps): $(objdir)/%.d: $(srcdir)/%.cpp
	$(info DEPS: $@)
	@mkdir -p $(objdir)
	@$(cxx) $(dependflags) $< | $(dependscript) > $@

.PHONY: clean
clean:
	@$(RM) $(objs) $(deps)

.PHONY: distclean
distclean:
	@$(RM) $(wildcard $(objdir)/*) $(TARGET)

-include $(deps)
