#include "blocks.hpp"
#include "interpreter.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f, const Log &log, Error &error)
    : log_{log}, error_{error} {
  Super::try_emplace(&f.getEntryBlock(), f);
  for (const auto &b : f) {
    Super::try_emplace(&b);
  }
}
Env &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
