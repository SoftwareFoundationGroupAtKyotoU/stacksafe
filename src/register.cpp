#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>

namespace stacksafe {

Register::Register(const Value& v) : Value{v} {
  assert(Value::kind() == Value::Kind::REGISTER);
}
Register Register::make(const llvm::Argument& a) {
  return Register{a};
}
Register Register::make(const llvm::Instruction& i) {
  return Register{i};
}
const Value& Register::value() const {
  return *this;
}

}  // namespace stacksafe
