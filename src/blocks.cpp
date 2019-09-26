#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include <cassert>
#include "interpret.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  Super::try_emplace(&f.getEntryBlock(), Env{f});
  for (auto& b : f) {
    Super::try_emplace(&b, Env{});
  }
}
const Env& Blocks::get(const llvm::BasicBlock* b) const {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
Env& Blocks::get(const llvm::BasicBlock* b) {
  return const_cast<Env&>(static_cast<const Blocks&>(*this).get(b));
}
Env Blocks::interpret(const llvm::BasicBlock* b) {
  return Interpreter::run(b, get(b));
}
bool Blocks::update(const llvm::BasicBlock* b, const Env& next) {
  auto& prev = get(b);
  if (prev.includes(next)) {
    return false;
  } else {
    prev.merge(next);
    return true;
  }
}

}  // namespace stacksafe
