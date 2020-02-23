#include "block.hpp"
#include <llvm/IR/BasicBlock.h>

namespace stacksafe {

Blocks Blocks::successors(BB b) {
  Blocks succs;
  const auto t = b->getTerminator();
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    succs.push_back(t->getSuccessor(i));
  }
  return succs;
}
Blocks Blocks::successors() const {
  Blocks succs;
  for (const auto& b : *this) {
    for (const auto& succ : successors(b)) {
      if (!contains(succ) && !succs.contains(succ)) {
        succs.push_back(succ);
      }
    }
  }
  return succs;
}
bool Blocks::contains(BB b) const {
  return std::find(begin(), end(), b) != end();
}
bool Blocks::is_loop() const {
  for (const auto& b : *this) {
    for (const auto& succ : successors(b)) {
      if (contains(succ)) {
        return true;
      }
    }
  }
  return false;
}

void Components::transfer(const Blocks& b, const Map& pred) {
  for (const auto& succ : b.successors()) {
    find(succ).merge(pred);
  }
}
Map& Components::find(BB b) {
  const auto pred = [b](const Pair& pair) {
    return std::get<0>(pair).contains(b);
  };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return std::get<1>(*it);
}

}  // namespace stacksafe
