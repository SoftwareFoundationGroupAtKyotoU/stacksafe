#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include "debug.hpp"
#include "env.hpp"
#include "interpret.hpp"
#include "verify.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  Params args;
  for (auto& a : f.args()) {
    args.insert(a);
  }
  Super::try_emplace(&f.getEntryBlock(), Env{cache_, args}.memory());
  for (auto& b : f) {
    Super::try_emplace(&b, Memory{});
  }
}
Memory Blocks::interpret(const llvm::BasicBlock* b) {
  Env env{cache_, get(b)};
  Interpreter{env}.visit(*b);
  return env.memory();
}
bool Blocks::update(const llvm::BasicBlock* b, const Memory& next) {
  return get(b).merge(next);
}
bool Blocks::verify(const llvm::BasicBlock* b) {
  Env env{cache_, get(b)};
  Interpreter{env}.visit(*b);
  return Verifier{env}.visit(*b);
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
