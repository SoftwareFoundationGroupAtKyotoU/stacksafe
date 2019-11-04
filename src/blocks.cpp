#include "blocks.hpp"
#include "interpret.hpp"
#include "log.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f, const Log &l) : cache_{f}, log_{l} {
  Super::try_emplace(&f.getEntryBlock(), cache_, f);
  for (const auto &b : f) {
    Super::try_emplace(&b, cache_);
  }
}
std::optional<Memory> Blocks::interpret(const llvm::BasicBlock &b) {
  Interpreter i{log_, get(b)};
  if (i.visit(b)) {
    return i.memory();
  } else {
    return std::nullopt;
  }
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
