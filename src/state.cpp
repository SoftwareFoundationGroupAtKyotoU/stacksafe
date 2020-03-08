#include "state.hpp"
#include <llvm/IR/Instructions.h>
#include "tarjan.hpp"

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b}, graph_{} {}
const Blocks& Component::blocks() const {
  return blocks_;
}
Graph& Component::graph() {
  return graph_;
}
bool Component::is_safe() const {
  const auto pred = [& self = *this](BB b) { return self.check_return(b); };
  return check_global() && std::all_of(blocks_.begin(), blocks_.end(), pred);
}
void Component::add_pred(const Graph& g) {
  preds_.push_back(&g);
}
void Component::transfer() {
  for (auto&& pred : preds_) {
    graph_.merge(*pred);
  }
}
bool Component::check_global() const {
  return !graph_.reachables(NodeSet{Node::get_global()}).has_local();
}
bool Component::check_return(BB b) const {
  const auto t = b->getTerminator();
  if (const auto i = llvm::dyn_cast<llvm::ReturnInst>(t)) {
    if (const auto v = i->getReturnValue()) {
      return !graph_.reachables(*v).has_local();
    }
  }
  return true;
}

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
