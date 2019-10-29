#include "blocks.hpp"
#include <llvm/Support/Debug.h>
#include "env.hpp"
#include "fabric.hpp"
#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"
#include "verify.hpp"

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f) {
  auto m = Env{cache_, f}.memory();
  for (const auto &b : f) {
    Super::try_emplace(&b, m);
  }
  STACKSAFE_DEBUG_LOG(log_ = std::make_unique<Log>(f));
}
Blocks::~Blocks() = default;
Memory Blocks::interpret(const llvm::BasicBlock *b) const {
  Env env{cache_, get(*b)};
  Interpreter{env}.visit(*b);
  return env.memory();
}
bool Blocks::update(const llvm::BasicBlock *b, const Memory &next) {
  return get(*b).merge(next);
}
bool Blocks::verify(const llvm::BasicBlock *b) const {
  Env env{cache_, get(*b)};
  Interpreter{env}.visit(*b);
  return Verifier{env}.visit(*b);
}
void Blocks::print(const llvm::BasicBlock *b, const Memory &next) const {
  STACKSAFE_DEBUG_LOG(log_->print(*b, get(*b).diff(next)));
}
void Blocks::finish(const llvm::Function &f) const {
  STACKSAFE_DEBUG_LOG(log_->print(f));
  for (const auto &b : f) {
    STACKSAFE_DEBUG_LOG(log_->print(b, get(b).diff(interpret(&b))));
  }
}
Memory &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}
const Memory &Blocks::get(const llvm::BasicBlock &b) const {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
