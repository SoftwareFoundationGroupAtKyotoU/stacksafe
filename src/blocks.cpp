#include "blocks.hpp"
#include "interpret.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f) : log_{f} {
  Super::try_emplace(&f.getEntryBlock(), f);
  for (const auto &b : f) {
    Super::try_emplace(&b);
  }
}
std::optional<Env> Blocks::interpret(const llvm::BasicBlock &b) {
  Interpreter i{log_, get(b)};
  if (i.visit(b)) {
    return i.env();
  } else {
    return std::nullopt;
  }
}
bool Blocks::update(const llvm::BasicBlock &b, const Env &next) {
  if (get(b).includes(next)) {
    return false;
  } else {
    get(b).merge(next);
    return true;
  }
}
Env &Blocks::get(const llvm::BasicBlock &b) {
  auto it = Super::find(&b);
  assert(it != Super::end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
