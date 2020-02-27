#include "state.hpp"

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b}, graph_{} {}
bool Component::check_global() const {
  NodeSet nodes;
  graph_.reachables(Node::get_global(), nodes);
  return !nodes.has_local();
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
