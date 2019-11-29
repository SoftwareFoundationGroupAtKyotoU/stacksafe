#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include <cstdlib>
#include "utility.hpp"

namespace stacksafe {

Register::Register(const llvm::Value* v) : reg_{v} {
  if (reinterpret_cast<std::uintptr_t>(reg_) & 0x1) {
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

}  // namespace stacksafe
