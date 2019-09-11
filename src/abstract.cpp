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

Abstract::Abstract(Log& log) : log_{log} {}
std::unique_ptr<Log> Abstract::interpret(const llvm::Function& f) {
  Symbol::reset();
  auto log = std::make_unique<Log>(f);
  if (log) {
    Abstract abstract{*log};
    for (auto& b : f) {
      abstract.blocks_.try_emplace(&b, Env{});
    }
    abstract.interpret(&f.getEntryBlock(), Env{f});
  }
  return std::move(log);
}
std::optional<Env> Abstract::update(const llvm::BasicBlock* b,
                                    const Env& pred) {
  if (auto it = blocks_.find(b); it != blocks_.end()) {
    auto& prev = it->second;
    if (!prev.includes(pred)) {
      prev.merge(pred);
      auto next = Interpret::run(b, prev);
      log_.add(b, prev, next);
      return next;
    }
  } else {
    llvm_unreachable("Error: unknown basicblock");
  }
  return std::nullopt;
}
void Abstract::interpret(const llvm::BasicBlock* b, const Env& pred) {
  if (auto next = update(b, pred)) {
    if (auto t = b->getTerminator()) {
      for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
        interpret(t->getSuccessor(j), *next);
      }
    }
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
