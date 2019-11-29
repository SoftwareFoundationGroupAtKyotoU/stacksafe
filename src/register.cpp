#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "utility.hpp"

namespace stacksafe {

Register::Register(const llvm::Value* v) : reg_{v} {
  if (least_significant_bit(reg_)) {
    fatal_error("Value representation may conflict");
  }
}
Register::Register(const llvm::Instruction& i) : Register{&i} {
  if (!is_register(i)) {
    fatal_error("invalid register");
  }
}
Register::Register(const llvm::Argument& a) : Register{&a} {}
const llvm::Value& Register::value() const {
  return *reg_;
}

}  // namespace stacksafe
