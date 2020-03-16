#include "graph.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

NodeSet::NodeSet() = default;
NodeSet::NodeSet(const Node& n) {
  Super::insert(n);
}
void NodeSet::merge(const NodeSet& nodes) {
  Super::insert(nodes.begin(), nodes.end());
}
bool NodeSet::element(const Node& n) const {
  return 0 != Super::count(n);
}
bool NodeSet::includes(const NodeSet& that) const {
  for (const auto& n : that) {
    if (element(n)) {
      continue;
    }
    return false;
  }
  return true;
}
bool NodeSet::has_local() const {
  for (const auto& n : *this) {
    if (n.is_local()) {
      return true;
    }
  }
  return false;
}

std::size_t Graph::size() const {
  return heap_.size() + stack_.size();
}
bool Graph::includes(const Graph& that) const {
  for (const auto& [tail, head] : that.heap_) {
    if (contains(tail, head)) {
      continue;
    }
    return false;
  }
  for (const auto& [tail, head] : that.stack_) {
    if (contains(*tail, head)) {
      continue;
    }
    return false;
  }
  return true;
}
bool Graph::contains(const Node& tail, const Node& head) const {
  const auto [lb, ub] = heap_.equal_range(tail);
  const auto p = [&head](const Heap::value_type& e) {
    return std::get<1>(e) == head;
  };
  return std::any_of(lb, ub, p);
}
bool Graph::contains(const llvm::Value& tail, const Node& head) const {
  const auto [lb, ub] = stack_.equal_range(&tail);
  const auto p = [&head](const Stack::value_type& e) {
    return std::get<1>(e) == head;
  };
  return std::any_of(lb, ub, p);
}
void Graph::connect(const Node& tail, const Node& head) {
  const auto [lb, ub] = heap_.equal_range(tail);
  const auto p = [&head](const Heap::value_type& e) {
    return std::get<1>(e) == head;
  };
  if (std::find_if(lb, ub, p) == ub) {
    heap_.emplace_hint(lb, tail, head);
  }
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
  const auto p = [&heads](const Heap::value_type& e) {
    heads.insert(std::get<1>(e));
  };
  for (const auto& tail : tails) {
    const auto [lb, ub] = heap_.equal_range(tail);
    std::for_each(lb, ub, p);
  }
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
