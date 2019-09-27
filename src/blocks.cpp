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
  auto env = get_env(b);
  Interpreter{env}.visit(*b);
  return env.memory();
}
bool Blocks::update(const llvm::BasicBlock* b, const Memory& next) {
  return get(b).merge(next);
}
bool Blocks::verify(const llvm::BasicBlock* b) {
  auto env = get_env(b);
  Interpreter{env}.visit(*b);
  return Verifier{env}.visit(*b);
}
void Blocks::print(const Log& log, const llvm::BasicBlock* b,
                   const Memory& next) const {
  auto prev = Super::find(b);
  assert(prev != Super::end() && "unknown basicblock");
  log.print(b, prev->second, next);
}
Memory& Blocks::get(const llvm::BasicBlock* b) {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
Env Blocks::get_env(const llvm::BasicBlock* b) {
  return Env{cache_, get(b)};
}

}  // namespace stacksafe
