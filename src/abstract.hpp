#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <map>
#include "env.hpp"

namespace llvm {
class BasicBlock;
}

namespace stacksafe {

class Abstract {
  std::map<llvm::BasicBlock*, Env> blocks_;
  void interpret(llvm::BasicBlock* b, const Env& e);
  bool update(llvm::BasicBlock* b, const Env& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
