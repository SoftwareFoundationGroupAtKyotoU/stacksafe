#include "register.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
  Register::Register(llvm::Value &value)
    : reg_(&value)
  {}
  std::size_t Register::hash() const {
    return std::hash<void *>{}(reg_);
  }
}
namespace std {
  size_t hash<stacksafe::Register>::operator()(stacksafe::Register r) const {
    return r.hash();
  }
}
