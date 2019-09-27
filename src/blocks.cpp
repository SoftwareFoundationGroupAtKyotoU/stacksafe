#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include "debug.hpp"
#include "env.hpp"
#include "interpret.hpp"
#include "verify.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  Super::try_emplace(&f.getEntryBlock(), Memory{f});
  for (auto& b : f) {
    Super::try_emplace(&b, Memory{});
  }
}
Memory Blocks::interpret(const llvm::BasicBlock* b) {
  return Interpreter::run(b, Env{get(b), cache_});
}
bool Blocks::update(const llvm::BasicBlock* b, const Memory& next) {
  auto& prev = get(b);
  if (prev.includes(next)) {
    return false;
  } else {
    prev.merge(next);
    return true;
  }
}
bool Blocks::verify(const llvm::BasicBlock* b) {
  auto result = Interpreter::run(b, Env{get(b), cache_});
  return Verifier::run(b, result);
}
void Blocks::print(Log& log, const llvm::BasicBlock* b, const Memory& next) {
  const auto& prev = get(b);
  log.print(b, prev, next);
}
Memory& Blocks::get(const llvm::BasicBlock* b) {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
