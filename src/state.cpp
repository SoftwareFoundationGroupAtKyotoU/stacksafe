#include "state.hpp"
#include <llvm/IR/Function.h>
#include "tarjan.hpp"

namespace stacksafe {

State::State(const llvm::Function& f) {
  for (const auto& c : Tarjan::run(f)) {
    Super::emplace_back(c);
  }
  for (auto&& c : *this) {
    for (const auto& succ : c.blocks().successors()) {
      find(succ).add_pred(c.graph());
    }
  }
  find(&f.getEntryBlock()).graph().init(f);
}
void State::transfer(Component& c) {
  for (const auto& succ : c.blocks().successors()) {
    find(succ).graph().merge(c.graph());
  }
}
bool State::is_safe() const {
  const auto pred = [](const Component& c) { return c.is_safe(); };
  return std::all_of(begin(), end(), pred);
}
Component& State::find(BB b) {
  const auto pred = [b](const Component& c) { return c.blocks().contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return *it;
}

}  // namespace stacksafe
