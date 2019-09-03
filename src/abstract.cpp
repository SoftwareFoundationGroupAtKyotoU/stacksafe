#include "abstract.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include "interpret.hpp"
#include "json.hpp"

namespace stacksafe {

bool Todo::print() const {
  if (Base::empty()) {
    return false;
  } else {
    llvm::errs() << "Following instructions are not supported yet:\n";
    for (auto& i : *this) {
      llvm::errs() << *i << "\n";
    }
    return true;
  }
}

void Abstract::interpret(llvm::Function& f) {
  auto b = &f.getEntryBlock();
  Env e{f};
  update(nullptr, e);
  interpret(b, e);
  for (auto b : log_) {
    llvm::errs() << Value{*b}.repr() << "\n";
  }
}
void Abstract::interpret(llvm::BasicBlock* b, const Env& e) {
  log_.push_back(b);
  Interpret i{e, todo_};
  i.visit(*b);
  if (todo_.print()) {
    return;
  }
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
void to_json(Json& j, const Abstract& x) {
  for (auto& [k, v] : x.blocks_) {
    j[k ? Value{*k}.repr() : ""] = v;
  }
}

}  // namespace stacksafe
