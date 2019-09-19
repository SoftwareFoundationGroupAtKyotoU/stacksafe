#include "abstract.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/Debug.h>
#include "interpret.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function& f) : func_{f} {
#define DEBUG_TYPE "log"
  LLVM_DEBUG(log_ = Log{f});
#undef DEBUG_TYPE
  Symbol::reset();
  for (auto& b : f) {
    blocks_.try_emplace(&b, Env{});
  }
}
auto Abstract::blocks() const -> const Blocks& { return blocks_; }
void Abstract::interpret() { interpret(&func_.getEntryBlock(), Env{func_}); }
void Abstract::print(llvm::raw_ostream&) const {}
void Abstract::interpret(const llvm::BasicBlock* b, const Env& pred) {
  if (auto next = update(b, pred)) {
    if (auto t = b->getTerminator()) {
      for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
        interpret(t->getSuccessor(j), *next);
      }
    }
  }
}
std::optional<Env> Abstract::update(const llvm::BasicBlock* b,
                                    const Env& pred) {
  if (auto it = blocks_.find(b); it != blocks_.end()) {
    auto& prev = it->second;
    if (!prev.includes(pred)) {
      prev.merge(pred);
      auto next = Interpreter::run(b, prev);
#define DEBUG_TYPE "log"
      LLVM_DEBUG(log_->print(b, prev, next));
#undef DEBUG_TYPE
      return next;
    }
  } else {
    stacksafe_unreachable("unknown basicblock", *b);
  }
  return std::nullopt;
}
void to_json(Json& j, const Abstract& x) {
  Json::object_t obj;
  for (auto& [k, v] : x.blocks()) {
    obj[k ? Value::make(*k).repr() : ""] = v;
  }
  j = obj;
}

}  // namespace stacksafe
