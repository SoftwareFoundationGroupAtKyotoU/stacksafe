#include "debug.hpp"
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {

LogBlock::LogBlock(const Env& p, const llvm::BasicBlock* b, const Env& n)
    : prev{p}, next{n}, block{b} {}

void unknown_instruction(llvm::Instruction& i) {
  llvm::errs() << "unknown: " << i << "\n";
}

}  // namespace stacksafe
