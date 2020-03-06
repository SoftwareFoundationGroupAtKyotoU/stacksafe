#include "graph.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "utility.hpp"

namespace stacksafe {

Edge::Edge(const Node& n) : Super{n, Node::get_constant()} {}
const Node& Edge::tail() const {
  return std::get<0>(*this);
}
const Node& Edge::head() const {
  return std::get<1>(*this);
}
auto Edge::pair() const -> const Super& {
  return *this;
}
bool operator==(const Edge& lhs, const Edge& rhs) {
  return lhs.pair() == rhs.pair();
}
bool operator<(const Edge& lhs, const Edge& rhs) {
  return lhs.pair() < rhs.pair();
}

std::size_t Graph::size() const {
  return heap_.size() + stack_.size();
}
void Graph::init(const llvm::Function& f) {
  const NodeSet g{Node::get_global()};
  connect(g, g);
  for (const auto& a : f.args()) {
    connect(a, g);
  }
}
void Graph::connect(const NodeSet& tails, const NodeSet& heads) {
  for (const auto& tail : tails) {
    heap_at(tail).merge(heads);
  }
}
void Graph::connect(const llvm::Value& tail, const NodeSet& heads) {
  assert(is_register(tail));
  stack_at(tail).merge(heads);
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
NodeSet& Graph::heap_at(const Node& tail) {
  const auto [it, _] = heap_.try_emplace(tail);
  return std::get<1>(*it);
}
NodeSet& Graph::stack_at(const llvm::Value& tail) {
  const auto [it, _] = stack_.try_emplace(&tail);
  return std::get<1>(*it);
}

}  // namespace stacksafe
