# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)

# macro for debug flags
define debug-flags =
$(subst -O2,-O0,$(subst -g1,-g3,$(subst -DNDEBUG,-DDEBUG,$(1))))
endef

.PHONY: all
all: debug

.PHONY: debug
debug: cxxflags := $(call debug-flags,$(cxxflags))
debug: $(objs)

.PHONY: release
release: $(objs)
