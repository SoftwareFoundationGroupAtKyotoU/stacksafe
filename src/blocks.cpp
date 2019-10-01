#include "blocks.hpp"
#include <llvm/IR/Function.h>
#include "env.hpp"
#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "verify.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  auto m = init_env(f).memory();
  for (const auto& b : f) {
    Super::try_emplace(&b, m);
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
Env Blocks::init_env(const llvm::Function& f) {
  Env env{cache_};
  auto g = Domain::global();
  for (const auto& a : f.args()) {
    env.insert(a, g);
  }
  for (const auto& b : f) {
    for (const auto& i : b) {
      if (check_register(i)) {
        env.insert(i, Domain{});
      }
    }
  }
  return env;
}

}  // namespace stacksafe
