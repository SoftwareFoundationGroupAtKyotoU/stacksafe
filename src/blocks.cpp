#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include <cassert>
#include "debug.hpp"
#include "interpret.hpp"
#include "verify.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  Super::try_emplace(&f.getEntryBlock(), Env{f});
  for (auto& b : f) {
    Super::try_emplace(&b, Env{});
  }
}
Env& Blocks::get(const llvm::BasicBlock* b) {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
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
bool Blocks::verify(const llvm::BasicBlock* b) {
  auto result = Interpreter::run(b, get(b));
  return Verifier::run(b, result);
}
void Blocks::print(Log& log, const llvm::BasicBlock* b, const Env& next) {
  const auto& prev = get(b);
  log.print(b, prev, next);
}

}  // namespace stacksafe
