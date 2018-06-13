# user configuration
TARGET ?= MyHello.so
PASS ?= myhello
CFLAGS ?=
CXXFLAGS ?= -Wall -Wextra -Wpedantic
LDFLAGS ?=
OPTFLAGS ?= -time-passes
# default option
RM ?= rm -f

# llvm/clang tools
llvm_config := llvm-config
cxx := clang++
cc := clang
opt := opt
# version specification
ifdef VERSION
append-version = $(eval $(var) := $($(var))-$(VERSION))
$(foreach var,llvm_config cxx cc opt,$(append-version))
endif
# llvm flags
llvm_cxxflags != $(llvm_config) --cxxflags
llvm_ldflags != $(llvm_config) --ldflags
# adapt options to arguments of clang
llvm_cxxflags += -Wno-unused-command-line-argument
from := -Wno-maybe-uninitialized
to := -Wno-sometimes-uninitialized
llvm_cxxflags := $(subst $(from),$(to),$(llvm_cxxflags))

cflags := $(CFLAGS)
cxxflags := $(CXXFLAGS)
ldflags := $(LDFLAGS)
optflags := $(OPTFLAGS)
# add llvm flags
cxxflags += $(llvm_cxxflags)
ldflags += $(llvm_ldflags)
# add mandatory flags
cflags += -c -S -emit-llvm
cxxflags += -c
ldflags += -shared
optflags += -analyze -load $(CURDIR)/$(TARGET) -$(PASS)

# collect sub-targets
srcs := $(wildcard *.cpp)
objs := $(srcs:%.cpp=%.o)
testsrcs := $(wildcard test/*.c)
testcases := $(testsrcs:%.c=%)

all: $(TARGET)

$(TARGET): $(objs)
	$(cxx) $(ldflags) -o $@ $^

%.o: %.cpp
	$(cxx) $(cxxflags) -o $@ $<

%.ll: %.c
	$(cc) $(cflags) -o $@ $<

test: $(testcases)

$(testcases): test/%: test/%.ll $(TARGET)
	$(opt) $(optflags) $<

distclean: clean clean/$(TARGET)

clean: clean/objs clean/tests

clean/$(TARGET):
	$(RM) $(TARGET)

clean/objs:
	$(RM) $(objs)

clean/tests:
	$(RM) $(wildcard test/*.ll)

.PHONY: all test $(testcases)
.PHONY: distclean clean clean/$(TARGET) clean/objs clean/tests
