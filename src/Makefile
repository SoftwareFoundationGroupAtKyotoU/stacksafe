# user-specified flags
CXXFLAGS := -Wall -Wextra -Wpedantic

# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)
# llvm flags
llvm-cxxflags != $(config) --cxxflags
# adapt flags of gcc to clang ones
llvm-cxxflags += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
llvm-cxxflags := $(subst $(from),$(to),$(llvm-cxxflags))
# build options
cxxflags := -c $(CXXFLAGS) $(llvm-cxxflags)

# collect sub-targets
srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
cleanobjs := $(addprefix clean/,$(objs))

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

%.o: %.cpp
	$(cxx) $(cxxflags) -o $@ $<

.PHONY: clean
clean: $(cleanobjs)

.PHONY: $(cleanobjs)
$(cleanobjs): clean/%:
	@$(RM) $*
