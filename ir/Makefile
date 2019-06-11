#!/usr/bin/make -f

CC := clang
CFLAGS := -S -emit-llvm

emit-prefix := emit
srcs != find $(CURDIR) -type f -name '*.c'
emits := $(srcs:$(CURDIR)/%.c=$(emit-prefix)/%)

.PHONY: $(emits)
$(emits): $(emit-prefix)/%:
	$(CC) $(CFLAGS) -c $*.c -o $(notdir $*.ll)
