#include "state.hpp"
#include <llvm/IR/Instructions.h>

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b}, graph_{} {}
const Blocks& Component::blocks() const {
  return blocks_;
}
bool Component::is_safe() const {
  const auto pred = [& self = *this](BB b) { return self.check_return(b); };
  return check_global() && std::all_of(blocks_.begin(), blocks_.end(), pred);
}
bool Component::check_global() const {
  NodeSet nodes;
  graph_.reachables(Node::get_global(), nodes);
  return !nodes.has_local();
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

void State::transfer(const Component& c) {
  for (const auto& succ : c.blocks_.successors()) {
    find(succ).merge(c.graph_);
  }
}
Graph& State::find(BB b) {
  const auto pred = [b](const Component& c) { return c.blocks_.contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return it->graph_;
}

}  // namespace stacksafe
