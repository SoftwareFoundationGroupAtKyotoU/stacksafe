#include "block.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

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

namespace dataflow {
namespace block {
std::vector<Component> Scc::solve(const llvm::Function &f) {
  auto tarjan = std::make_unique<Scc>();
  for (const auto &b : f) {
    tarjan->push_back(&b);
  }
  const auto result = tarjan->apply();
  std::vector<Component> vec;
  for (const auto &v : result) {
    Component c;
    for (const auto &p : v) {
      c.push_back(static_cast<const llvm::BasicBlock *>(p));
    }
    vec.push_back(std::move(c));
  }
  std::reverse(vec.begin(), vec.end());
  return vec;
}
auto Scc::successors(Ptr p) const -> Vec {
  Vec vec;
  if (auto b = static_cast<const llvm::BasicBlock *>(p)) {
    const auto t = b->getTerminator();
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      vec.push_back(t->getSuccessor(i));
    }
  }
  return vec;
}
}  // namespace block
}  // namespace dataflow
