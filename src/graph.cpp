#include "graph.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

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
  return map_.size();
}
void Graph::init(const llvm::Function& f) {
  const auto g = Node::get_global();
  connect(g, g);
  for (const auto& a : f.args()) {
    const auto reg = Node::get_register(a);
    connect(reg, g);
  }
}
void Graph::connect(const Node& tail, const Node& head) {
  const auto it = std::get<0>(map_.try_emplace(tail));
  std::get<1>(*it).insert(head);
}
NodeSet Graph::followings(const Node& tail) const {
  NodeSet nodes;
  if (const auto it = map_.find(tail); it != map_.end()) {
    const auto& heads = std::get<1>(*it);
    nodes.insert(heads.begin(), heads.end());
  }
  return nodes;
}
void Graph::merge(const Graph& g) {
  for (const auto& [tail, heads] : g.map_) {
    const auto it = std::get<0>(map_.try_emplace(tail));
    std::get<1>(*it).insert(heads.begin(), heads.end());
  }
}
void Graph::reachables(const Node& n, NodeSet& nodes) const {
  if (!nodes.element(n)) {
    nodes.insert(n);
    for (const auto& head : followings(n)) {
      reachables(head, nodes);
    }
  }
}
void Graph::reachables(const llvm::Value& v, NodeSet& nodes) const {
  for (const auto& tail : followings(Node::from_value(v))) {
    reachables(tail, nodes);
  }
}

}  // namespace stacksafe
