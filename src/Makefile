# llvm/clang tools
ifdef CLANG_VERSION
suffix := -$(CLANG_VERSION)
endif
config := llvm-config$(suffix)
cxx := clang++$(suffix)

.PHONY: all
all: debug

.PHONY: debug
debug: $(objs)

.PHONY: release
release: $(objs)
