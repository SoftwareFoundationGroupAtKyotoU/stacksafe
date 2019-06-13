#!/usr/bin/make -f

LLVM_SUFFIX ?=
PASS ?= stacksafe
TARGET_PATH ?= $(CURDIR)/../stacksafe.so
CC := clang
CFLAGS := -S -emit-llvm

opt := opt$(LLVM_SUFFIX)
optflags := -analyze -load=$(TARGET_PATH) -$(PASS)
# optflags += -time-passes

emit-prefix := emit
test-prefix := test
run-prefix := run
srcs != find $(CURDIR) -type f -name '*.c'
objs := $(wildcard *.ll)
emits := $(srcs:$(CURDIR)/%.c=$(emit-prefix)/%)
tests := $(objs:%.ll=$(test-prefix)/%)
runs := $(objs:%.ll=$(run-prefix)/%)

.PHONY: $(emits)
$(emits): $(emit-prefix)/%:
	$(CC) $(CFLAGS) -c $*.c -o $(notdir $*.ll)

.PHONY: $(tests)
$(tests): optflags += -debug
$(tests): $(test-prefix)/%: $(run-prefix)/%

.PHONY: $(runs)
$(runs): $(run-prefix)/%: %.ll
	@echo ---- $* begins ----
	$(opt) $(optflags) $<
	@echo ---- $* ends   ----
