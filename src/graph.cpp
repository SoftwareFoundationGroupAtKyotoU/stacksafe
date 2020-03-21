#include "graph.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

std::size_t Graph::size() const {
  return map_.size() + stack_.size();
}
void Graph::merge(const Graph& g) {
  map_.merge(g.map_);
}
bool Graph::includes(const Graph& that) const {
  for (const auto& [tail, head] : that.stack_) {
    if (contains(*tail, head)) {
      continue;
    }
    return false;
  }
  return true;
}
bool Graph::contains(const Node& tail, const Node& head) const {
  return map_.exists(tail, head);
}
bool Graph::contains(const llvm::Value& tail, const Node& head) const {
  const auto [lb, ub] = stack_.equal_range(&tail);
  const auto p = [&head](const Stack::value_type& e) {
    return std::get<1>(e) == head;
  };
  return std::any_of(lb, ub, p);
}
void Graph::connect(const Node& tail, const Node& head) {
  map_.add(tail, head);
}
void Graph::connect(const llvm::Value& tail, const Node& head) {
  const auto [lb, ub] = stack_.equal_range(&tail);
  const auto p = [&head](const Stack::value_type& e) {
    return std::get<1>(e) == head;
  };
  if (std::find_if(lb, ub, p) == ub) {
    stack_.emplace_hint(lb, &tail, head);
  }
}
void Graph::followings(const NodeSet& tails, NodeSet& heads) const {
  for (const auto& tail : tails) {
    heads.merge(map_.lookup(tail));
  }
}
void Graph::followings(const llvm::Value& tail, NodeSet& heads) const {
  if (is_global(tail)) {
    followings(NodeSet{Node::get_global()}, heads);
  } else {
    const auto p = [&heads](const Stack::value_type& e) {
      heads.insert(std::get<1>(e));
    };
    const auto [lb, ub] = stack_.equal_range(&tail);
    std::for_each(lb, ub, p);
  }
}
NodeSet Graph::reachables(const NodeSet& nodes) const {
  NodeSet tails, heads{nodes};
  while (tails.size() < heads.size()) {
    tails = heads;
    followings(tails, heads);
  }
  return heads;
}
NodeSet Graph::reachables(const llvm::Value& value) const {
  NodeSet nodes;
  followings(value, nodes);
  return reachables(nodes);
}

}  // namespace stacksafe
