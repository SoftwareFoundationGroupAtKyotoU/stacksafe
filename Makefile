# target info
export TARGET := StackSafe.so
export PASS := stacksafe
export CLANG_VERSION := 6.0
# user-specified flags
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS :=

# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)
# llvm flags
llvm-cxxflags != $(config) --cxxflags | sed -e 's/-std=[^ ]*//g'
llvm-ldflags != $(config) --ldflags
# adapt flags of gcc to clang ones
llvm-cxxflags += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
llvm-cxxflags := $(subst $(from),$(to),$(llvm-cxxflags))
# build options
releaseflags := -c $(CXXFLAGS) $(llvm-cxxflags)
debugflags := $(releaseflags)
debugflags := $(subst -O2,-O0,$(debugflags))
debugflags := $(subst -g1,-g3,$(debugflags))
debugflags := $(subst -DNDEBUG,-DDEBUG,$(debugflags))
cxxflags = $($(shell $(switch-script))flags)
ldflags := -shared $(LDFLAGS) $(llvm-ldflags)

# collect sub-targets
debugmarker := .debug
srcprefix := src/
srcs := $(wildcard $(srcprefix)*.cpp)
objs := $(srcs:%.cpp=%.o)
cleanobjs := $(addprefix clean/,$(debugmarker) $(objs))

# script snippets
quiet-make := $(MAKE) --no-print-directory
check-debug := test -e $(debugmarker)
common-part := test -e .debug; then $(quiet-make) clean
debug-script := if ! $(common-part); touch $(debugmarker); fi
release-script := if $(common-part); fi
switch-script := if $(check-debug); then echo debug; else echo release; fi
sed-script := sed -e 's%/usr/lib/[^ ]*%%g'
define dependency-script =
$(shell echo $(srcprefix)$(shell $(cxx) $(cxxflags) -MM $(1) | $(sed-script)))
endef

.SUFFIXES:

.PHONY: all
all: build test

.PHONY: build
build: $(TARGET)

$(TARGET): $(objs)
	$(cxx) $(ldflags) -o $@ $^

.PHONY: debug release
debug release: %:
	@$($*-script)
	@$(quiet-make) build

$(objs): %.o: %.cpp
	$(cxx) $(cxxflags) -o $@ $<

.PHONY: test
test: $(TARGET)
	@$(MAKE) -C test all

.PHONY: distclean
distclean: clean clean/$(TARGET)

.PHONY: clean
clean: $(cleanobjs) clean/test

.PHONY: clean/$(TARGET) $(cleanobjs)
clean/$(TARGET) $(cleanobjs): clean/%:
	@$(RM) $*

.PHONY: clean/test
clean/test: clean/%:
	@$(quiet-make) -C $* clean

# dependency rules
$(foreach src,$(srcs),$(eval $(call dependency-script,$(src))))
