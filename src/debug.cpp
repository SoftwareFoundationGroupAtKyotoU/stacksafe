#include "debug.hpp"
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {

void unknown_instruction(llvm::Instruction& i) {
  llvm::errs() << "unknown: " << i << "\n";
}

}  // namespace stacksafe
