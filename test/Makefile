# user-specified flags
CFLAGS :=
OPTFLAGS := #-time-passes

# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cc := clang$(suffix)
opt := opt$(suffix)
# auxiliary options
load-option := -load $(dir $(CURDIR))$(TARGET)
pass-option := -$(PASS)
# build options
cflags := -c -S -emit-llvm $(CFLAGS)
optflags := -analyze $(load-option) $(pass-option) $(OPTFLAGS)

# collect sub-targets
srcs := $(wildcard *.c)
tests := $(srcs:%.c=%)
cleans := $(addprefix clean/,$(tests))

.PHONY: all
all: $(tests)

%.ll: %.c
	$(cc) $(cflags) -o $@ $<

.PHONY: $(tests)
$(tests): %: %.ll
	@echo ---- $< begins ----
	$(opt) $(optflags) $<
	@echo ---- $< ends ----
	@echo

.PHONHY: clean
clean: $(cleans)

.PHONY: $(cleans)
$(cleans): clean/%:
	@$(RM) $*.ll
