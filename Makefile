# target info
export TARGET := MyHello.so
export PASS := myhello
export VERSION := 4.0
# user-specified flags
CXXFLAGS := -Wall -Wextra -Wpedantic
LDFLAGS :=
# default option
RM ?= rm -f

# llvm/clang tools
ifdef VERSION
suffix := -$(VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)
# llvm flags
llvm-cxxflags != $(config) --cxxflags
llvm-ldflags != $(config) --ldflags
# adapt flags of gcc to clang ones
llvm-cxxflags += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
llvm-cxxflags := $(subst $(from),$(to),$(llvm-cxxflags))
# build options
cxxflags := -c $(CXXFLAGS) $(llvm-cxxflags)
ldflags := -shared $(LDFLAGS) $(llvm-ldflags)

# collect sub-targets
srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
cleanobjs := $(addprefix clean/,$(objs))

.PHONY: all
all: test

.PHONY: build
build: debug

.PHONY: debug
debug: cxxflags := $(subst -DNDEBUG,-DDEBUG,$(cxxflags))
debug: cxxflags := $(subst -g1,-g3,$(cxxflags))
debug: cxxflags := $(subst -O2,-O0,$(cxxflags))
debug: $(TARGET)

.PHONY: release
release: $(TARGET)

$(TARGET): $(objs)
	$(cxx) $(ldflags) -o $@ $^

%.o: %.cpp
	$(cxx) $(cxxflags) -o $@ $<

.PHONY: test
test: build
	$(MAKE) -C test test

.PHONY: distclean
distclean: clean clean/$(TARGET)

.PHONY: clean
clean: $(cleanobjs) clean/test

.PHONY: clean/$(TARGET) $(cleanobjs)
clean/$(TARGET) $(cleanobjs): clean/%:
	@$(RM) $*

.PHONY: clean/test
clean/test:
	@$(MAKE) --no-print-directory -C test clean
