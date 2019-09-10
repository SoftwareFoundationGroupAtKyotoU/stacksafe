#include "abstract.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include "debug.hpp"
#include "interpret.hpp"
#include "json.hpp"
#include "token.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function& f, Log& log) : func_{&f}, log_{log} {
  Symbol::reset();
}
void Abstract::interpret() {
  auto& f = const_cast<llvm::Function&>(*func_);
  interpret(&f.getEntryBlock(), Env{*func_});
}
void Abstract::interpret(llvm::BasicBlock* b, const Env& e) {
  auto next = e;
  Interpret{next}.visit(*b);
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
bool Abstract::update(llvm::BasicBlock* b, const Env& e) {
  if (auto it = blocks_.find(b); it != blocks_.end()) {
    return it->second.merge(e);
  } else {
    blocks_.try_emplace(b, e);
    return true;
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
