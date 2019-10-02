#include "blocks.hpp"
#include "env.hpp"
#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "verify.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  auto m = Env{cache_, f}.memory();
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
void Blocks::finish(const Log& log, const llvm::Function& f) {
  log.print(f);
  for (const auto& b : f) {
    print(log, &b, interpret(&b));
  }
}
Memory& Blocks::get(const llvm::BasicBlock* b) {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
const Memory& Blocks::get(const llvm::BasicBlock* b) const {
  auto it = Super::find(b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
Env Blocks::get_env(const llvm::BasicBlock* b) const {
  return Env{cache_, get(b)};
}

}  // namespace stacksafe
