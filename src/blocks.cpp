#include "blocks.hpp"
#include <llvm/IR/Function.h>

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f, MapPool &pool) : pool_{pool} {
  for (const auto &b : f) {
    Super::try_emplace(&b);
  }
}
Env &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
void Blocks::merge(const llvm::BasicBlock &dst, const FlatEnv &src) {
  get(dst).merge(pool_.add(src));
}
void Blocks::merge(const llvm::BasicBlock &dst, const llvm::BasicBlock &src) {
  get(dst).merge(get(src));
}

}  // namespace stacksafe
