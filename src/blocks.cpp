#include "blocks.hpp"
#include <llvm/Support/Debug.h>
#include "fabric.hpp"
#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f, const Log &l)
    : cache_{f}, log_{l}, error_{false} {
  auto g = Domain::global();
  Memory entry;
  entry.insert(Symbol::global(), g);
  for (const auto &a : f.args()) {
    entry.insert(cache_.lookup(a), g);
  }
  Super::try_emplace(&f.getEntryBlock(), entry);
  for (const auto &b : f) {
    Super::try_emplace(&b, Memory{});
  }
}
bool Blocks::is_error() const {
  return error_;
}
Memory Blocks::interpret(const llvm::BasicBlock &b) {
  Interpreter i{cache_, log_, get(b)};
  if (i.visit(b)) {
    error_ = true;
  }
  return i.memory();
}
bool Blocks::update(const llvm::BasicBlock &b, const Memory &next) {
  if (get(b).includes(next)) {
    return false;
  } else {
    get(b).merge(next);
    return true;
  }
}
Memory &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
