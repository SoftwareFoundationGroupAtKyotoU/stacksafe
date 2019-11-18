#include "blocks.hpp"
#include "interpreter.hpp"

namespace stacksafe {

Blocks::Blocks(const llvm::Function &f, Error &error) : log_{f}, error_{error} {
  Super::try_emplace(&f.getEntryBlock(), f);
  for (const auto &b : f) {
    Super::try_emplace(&b);
  }
}
Env Blocks::interpret(const llvm::BasicBlock &b) {
  Interpreter i{log_, error_, get(b)};
  i.visit(b);
  return i.env();
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
