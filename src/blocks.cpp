#include "blocks.hpp"
#include <cassert>

namespace stacksafe {

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
