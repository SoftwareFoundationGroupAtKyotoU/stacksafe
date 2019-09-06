#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include "env.hpp"

namespace llvm {
class BasicBlock;
}

namespace stacksafe {

struct LogBlock {
  const Env before, after;
  llvm::BasicBlock* block;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
