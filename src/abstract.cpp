#include "abstract.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include "interpret.hpp"

namespace stacksafe {

void Abstract::interpret(llvm::Function& f) {
  auto b = &f.getEntryBlock();
  interpret(b, Env{f});
}
void Abstract::interpret(llvm::BasicBlock* b, const Env& e) {
  Interpret i{e};
  i.visit(*b);
  auto& next = i.get();
  if (update(b, next)) {
    if (auto t = b->getTerminator()) {
      for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
        auto succ = t->getSuccessor(j);
        interpret(succ, next);
      }
    }
  }
}
bool Abstract::update(llvm::BasicBlock* b, const Env& e) {
  if (auto it = blocks_.find(b); it != blocks_.end()) {
    return it->second.merge(e);
  } else {
    blocks_.try_emplace(b, e);
    return true;
  }
}

}  // namespace stacksafe
