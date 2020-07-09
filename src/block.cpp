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
  for (const auto &b : *this) {
    for (const auto &succ : successors(b)) {
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
  for (const auto &b : *this) {
    for (const auto &succ : successors(b)) {
      if (contains(succ)) {
        return true;
      }
    }
  }
  return false;
}

}  // namespace stacksafe
