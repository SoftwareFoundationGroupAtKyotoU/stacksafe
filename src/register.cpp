#include "register.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include <cstdlib>
#include "utility.hpp"

namespace stacksafe {

Register::Register(const llvm::Instruction& i) : reg_{&i} {
  if (reinterpret_cast<std::uintptr_t>(reg_) & 0x1) {
    debug::print("ERROR: Value representation may conflict");
    std::exit(EXIT_FAILURE);
  } else if (!is_register(i)) {
    debug::print("ERROR: invalid register");
    std::exit(EXIT_FAILURE);
  }
}
Register::Register(const llvm::Argument& a) : reg_{&a} {}

}  // namespace stacksafe
