#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include <cstdlib>
#include "utility.hpp"

namespace stacksafe {

Register::Register(const llvm::Value* v) : reg_{v} {
  if (least_significant_bit(reg_)) {
    debug::print("ERROR: Value representation may conflict");
    std::exit(EXIT_FAILURE);
  }
}
Register::Register(const llvm::Instruction& i) : Register{&i} {
  if (!is_register(i)) {
    debug::print("ERROR: invalid register");
    std::exit(EXIT_FAILURE);
  }
}
Register::Register(const llvm::Argument& a) : Register{&a} {}
const llvm::Value& Register::value() const {
  return *reg_;
}

}  // namespace stacksafe
