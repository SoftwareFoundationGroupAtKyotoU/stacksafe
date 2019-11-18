#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "utility.hpp"

namespace stacksafe {

Register::Register(const Value& v) : Value{v} {
  assert(is_argument(*v.get()) || is_register(*v.get()));
}
Register Register::make(const llvm::Instruction& i) {
  return Register{i};
}
const Value& Register::value() const {
  return *this;
}

}  // namespace stacksafe
