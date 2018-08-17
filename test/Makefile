# user configuration point
TARGET := $(dir $(CURDIR))MyHello.so
PASS := myhello
CFLAGS :=
OPTFLAGS := # -time-passes
VERSION := 4.0
# default option
RM ?= rm -f

# llvm/clang tools
ifdef VERSION
suffix := -$(VERSION)
endif
config := llvm-config$(suffix)
cc := clang$(suffix)
opt := opt$(suffix)
# build options
cflags := -c -S -emit-llvm $(CFLAGS)
optflags := -analyze -load $(TARGET) -$(PASS) $(OPTFLAGS)

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
