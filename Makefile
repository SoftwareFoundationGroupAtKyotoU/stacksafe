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

.PHONY: all
all: debug test

.PHONY: build
build:
	$(cxx) $(ldflags) -o $(TARGET) $(wildcard src/*.o)

.PHONY: compile/debug compile/release
compile/debug compile/release: compile/%:
	$(MAKE) -C src $*

.PHONY: debug
debug: compile/debug build

.PHONY: release
release: compile/release build

.PHONY: test
test:
	$(MAKE) -C test all

.PHONY: distclean
distclean: clean clean/$(TARGET)

.PHONY: clean
clean: clean/src clean/test

.PHONY: clean/$(TARGET)
clean/$(TARGET):
	@$(RM) $(TARGET)

.PHONY: clean/test clean/src
clean/test clean/src: clean/%:
	@$(MAKE) --no-print-directory -C $* clean
