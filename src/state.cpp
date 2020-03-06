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
bool Component::check_global() const {
  return !graph_.reachables(NodeSet{Node::get_global()}).has_local();
}
bool Component::check_return(BB b) const {
  const auto t = b->getTerminator();
  if (const auto i = llvm::dyn_cast<llvm::ReturnInst>(t)) {
    if (const auto v = i->getReturnValue()) {
      NodeSet ret;
      graph_.reachables(*v, ret);
      return !ret.has_local();
    }
  }
  return true;
}

State::State(const llvm::Function& f) {
  for (const auto& c : Tarjan::run(f)) {
    Super::emplace_back(c);
  }
  find(&f.getEntryBlock()).init(f);
}
void State::transfer(const Component& c) {
  for (const auto& succ : c.blocks_.successors()) {
    find(succ).merge(c.graph_);
  }
}
bool State::is_safe() const {
  const auto pred = [](const Component& c) { return c.is_safe(); };
  return std::all_of(begin(), end(), pred);
}
Graph& State::find(BB b) {
  const auto pred = [b](const Component& c) { return c.blocks_.contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return it->graph_;
}

}  // namespace stacksafe
