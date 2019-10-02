#include "blocks.hpp"
#include <llvm/Support/Debug.h>
#include "env.hpp"
#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "verify.hpp"

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {

Blocks::Blocks(const llvm::Function& f) {
  auto m = Env{cache_, f}.memory();
  for (const auto& b : f) {
    Super::try_emplace(&b, m);
  }
}
Memory Blocks::interpret(const llvm::BasicBlock* b) const {
  Env env{cache_, get(b)};
  Interpreter{env}.visit(*b);
  return env.memory();
}
bool Blocks::update(const llvm::BasicBlock* b, const Memory& next) {
  return get(b).merge(next);
}
bool Blocks::verify(const llvm::BasicBlock* b) const {
  Env env{cache_, get(b)};
  Interpreter{env}.visit(*b);
  return Verifier{env}.visit(*b);
}
void Blocks::print(const Log& log, const llvm::BasicBlock* b,
                   const Memory& next) const {
  log.print(b, get(b), next);
}
void Blocks::finish(const Log& log, const llvm::Function& f) const {
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

}  // namespace stacksafe
