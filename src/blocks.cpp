#include "blocks.hpp"
#include <llvm/IR/Function.h>

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f) {
  Super::try_emplace(&f.getEntryBlock(), f);
  for (const auto &b : f) {
    Super::try_emplace(&b);
  }
}
EnvSlice &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
