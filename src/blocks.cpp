#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include <cassert>

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  Super::try_emplace(&f.getEntryBlock(), Env{f});
  for (auto& b : f) {
    Super::try_emplace(&b, Env{});
  }
}
void Blocks::init(const llvm::BasicBlock* b) { Super::try_emplace(b, Env{}); }
const Env& Blocks::get(const llvm::BasicBlock* b) const {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
Env& Blocks::get(const llvm::BasicBlock* b) {
  return const_cast<Env&>(static_cast<const Blocks&>(*this).get(b));
}

}  // namespace stacksafe
