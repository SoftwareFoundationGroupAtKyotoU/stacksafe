#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include "env.hpp"

namespace llvm {
class BasicBlock;
class Instruction;
}  // namespace llvm

namespace stacksafe {

struct LogBlock {
  const Env prev, next;
  const llvm::BasicBlock* const block;

 public:
  LogBlock(const Env& p, const llvm::BasicBlock* b, const Env& n);
};

struct Log {
  std::vector<LogBlock> blocks;
};

void unknown_instruction(llvm::Instruction& i);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
