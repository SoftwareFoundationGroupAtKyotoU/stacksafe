#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>

namespace stacksafe {

Register::Register(const llvm::Argument& v) : Value{&v} {}
Register::Register(const llvm::Instruction& v) : Value{&v} {}
Register::Register(const llvm::Value& v) : Value{&v} {}
Register::Register(const Value& v) : Value{v} {}

}  // namespace stacksafe
