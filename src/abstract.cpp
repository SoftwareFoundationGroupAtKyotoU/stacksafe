#include "abstract.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/ErrorHandling.h>
#include "debug.hpp"
#include "interpret.hpp"
#include "json.hpp"
#include "token.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function& f, Log& log) : func_{&f}, log_{log} {
  Symbol::reset();
  Env empty;
  for (auto& b : f) {
    prevs_.try_emplace(&b, empty);
  }
}
void Abstract::interpret() { process(&func_->getEntryBlock(), Env{*func_}); }
void Abstract::interpret(const llvm::BasicBlock* b, const Env& e) {
  auto next = Interpret::run(b, e);
  log_.add(e, b, next);
  if (update(b, next)) {
    if (auto t = b->getTerminator()) {
      for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
        auto succ = t->getSuccessor(j);
        interpret(succ, next);
      }
    }
  }
}
bool Abstract::update(const llvm::BasicBlock* b, const Env& e) {
  if (auto it = blocks_.find(b); it != blocks_.end()) {
    return it->second.merge(e);
  } else {
    blocks_.try_emplace(b, e);
    return true;
  }
}
void Abstract::process(const llvm::BasicBlock* b, const Env& pred) {
  if (auto it = prevs_.find(b); it != prevs_.end()) {
    auto& prev = it->second;
    if (!prev.includes(pred)) {
      prev.merge(pred);
      auto next = Interpret::run(b, prev);
      log_.add(prev, b, next);
      if (auto t = b->getTerminator()) {
        for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
          process(t->getSuccessor(j), next);
        }
      }
    }
  } else {
    llvm_unreachable("Error: unknown basicblock");
  }
}
void to_json(Json& j, const Abstract& x) {
  Json::object_t obj;
  for (auto& [k, v] : x.blocks_) {
    obj[k ? Value::create(*k).repr() : ""] = v;
  }
  j = obj;
}

}  // namespace stacksafe
