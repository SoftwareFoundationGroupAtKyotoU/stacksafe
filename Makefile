# target info
export TARGET := MyHello.so
export PASS := myhello
export CLANG_VERSION := 4.0
# user-specified flags
LDFLAGS :=
# default option
RM ?= rm -f

# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)
# llvm flags
llvm-ldflags != $(config) --ldflags
# build options
ldflags := -shared $(LDFLAGS) $(llvm-ldflags)

# macro for check .debug
define check-debug =
$(shell if test -e .debug; then echo debug; else echo release; fi)
endef
# auxiliary commands
noenter-make := $(MAKE) --no-print-directory
common-part := test -e .debug; then $(noenter-make) clean/src

.PHONY: all
all: test

$(TARGET): compile
	$(cxx) $(ldflags) -o $@ $(wildcard src/*.o)

.PHONY: compile
compile:
	@$(noenter-make) compile/$(call check-debug)

.PHONY: compile/debug compile/release
compile/debug compile/release: compile/%:
	$(MAKE) -C src $*

.PHONY: debug release
debug:
	@if ! $(common-part); touch .debug; fi
	@$(noenter-make) compile
release:
	@if $(common-part); fi
	@$(noenter-make) compile

.PHONY: test
test: $(TARGET)
	$(MAKE) -C test all

.PHONY: distclean
distclean: clean clean/$(TARGET)

.PHONY: clean
clean: clean/src clean/test

.PHONY: clean/$(TARGET) clean/.debug
clean/$(TARGET) clean/.debug: clean/%:
	@$(RM) $*

.PHONY: clean/test clean/src
clean/src: clean/.debug
clean/test clean/src: clean/%:
	@$(noenter-make) -C $* clean
