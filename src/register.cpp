#include "register.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
  Register::Register(llvm::Value &value)
    : reg_(&value)
  {}
}
