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
  for (const auto& [tail, heads] : that.heap_) {
    for (const auto& head : heads) {
      if (contains(tail, head)) {
        continue;
      }
      return false;
    }
  }
  for (const auto& [tail, heads] : that.stack_) {
    for (const auto& head : heads) {
      if (contains(*tail, head)) {
        continue;
      }
      return false;
    }
  }
  return true;
}
bool Graph::contains(const Node& tail, const Node& head) const {
  if (const auto it = heap_.find(tail); it != heap_.end()) {
    return std::get<1>(*it).element(head);
  }
  return false;
}
bool Graph::contains(const llvm::Value& tail, const Node& head) const {
  if (const auto it = stack_.find(&tail); it != stack_.end()) {
    return std::get<1>(*it).element(head);
  }
  return false;
}
void Graph::connect(const Node& tail, const Node& head) {
  heap_at(tail).merge(NodeSet{head});
}
void Graph::connect(const llvm::Value& tail, const Node& head) {
  assert(is_register(tail));
  stack_at(tail).merge(NodeSet{head});
}
void Graph::followings(const NodeSet& tails, NodeSet& heads) const {
  for (const auto& tail : tails) {
    if (const auto it = heap_.find(tail); it != heap_.end()) {
      heads.merge(std::get<1>(*it));
    }
  }
}
void Graph::followings(const llvm::Value& tail, NodeSet& heads) const {
  if (is_global(tail)) {
    followings(NodeSet{Node::get_global()}, heads);
  } else if (const auto it = stack_.find(&tail); it != stack_.end()) {
    heads.merge(std::get<1>(*it));
  }
}
void Graph::merge(const Graph& g) {
  for (const auto& [tail, heads] : g.heap_) {
    heap_at(tail).merge(heads);
  }
  for (const auto& [tail, heads] : g.stack_) {
    stack_at(*tail).merge(heads);
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
NodeSet Graph::reachables(const llvm::Value& v) const {
  NodeSet nodes;
  followings(v, nodes);
  return reachables(nodes);
}
const NodeSet* Graph::find(const Node& tail) const {
  if (const auto it = heap_.find(tail); it != heap_.end()) {
    return &std::get<1>(*it);
  }
  return nullptr;
}
const NodeSet* Graph::find(const llvm::Value& tail) const {
  if (const auto it = stack_.find(&tail); it != stack_.end()) {
    return &std::get<1>(*it);
  }
  return nullptr;
}
NodeSet& Graph::heap_at(const Node& tail) {
  const auto [it, _] = heap_.try_emplace(tail);
  return std::get<1>(*it);
}
NodeSet& Graph::stack_at(const llvm::Value& tail) {
  const auto [it, _] = stack_.try_emplace(&tail);
  return std::get<1>(*it);
}

}  // namespace stacksafe
