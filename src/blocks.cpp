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

Blocks::Blocks(const llvm::Function &f, Log &l)
    : cache_{f}, log_{l}, error_{false} {
  auto m = Env{cache_, f, log_}.memory();
  for (const auto &b : f) {
    Super::try_emplace(&b, m);
  }
}
Blocks::~Blocks() = default;
bool Blocks::is_error() const {
  return error_;
}
Memory Blocks::interpret(const llvm::BasicBlock &b) {
  Interpreter i{cache_, log_};
  if (i.visit(b)) {
    error_ = true;
  }
  return i.memory();
}
bool Blocks::update(const llvm::BasicBlock &b, const Memory &next) {
  return get(b).merge(next);
}
bool Blocks::verify(const llvm::BasicBlock &b) const {
  Env env{cache_, get(b), log_};
  Interpreter{cache_, log_}.visit(b);
  return Verifier{env}.visit(b);
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
