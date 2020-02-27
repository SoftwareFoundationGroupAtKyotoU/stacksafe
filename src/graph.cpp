#include "graph.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {
namespace {
bool tail_cmp(const Edge& lhs, const Edge& rhs) {
  return lhs.tail() < rhs.tail();
}
}  // namespace

Edge::Edge(const Node& n) : Super{n, Node{}} {}
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

void Graph::init(const llvm::Function& f) {
  const auto g = Node::get_global();
  append(g, g);
  for (const auto& a : f.args()) {
    const auto reg = Node::get_register(a);
    append(reg, g);
  }
}
bool Graph::append(const Node& tail, const Node& head) {
  if (head.is_symbol()) {
    return std::get<1>(insert(begin(), Edge{tail, head}));
  }
  return false;
}
NodeSet Graph::heads(const Node& tail) const {
  NodeSet nodes;
  const auto [lb, ub] = std::equal_range(begin(), end(), Edge{tail}, tail_cmp);
  for (auto it = lb; it != ub; ++it) {
    nodes.insert(it->head());
  }
  return nodes;
}
NodeSet Graph::tails() const {
  NodeSet nodes;
  for (const auto& e : *this) {
    nodes.insert(e.tail());
  }
  return nodes;
}
bool Graph::merge(const Graph& g) {
  iterator hint = begin();
  bool updated = false;
  for (const auto& e : g) {
    const auto [it, ok] = insert(hint, e);
    hint = std::next(it);
    if (ok) {
      updated = true;
    }
  }
  return updated;
}
void Graph::reachables(const Node& n, NodeSet& nodes) const {
  if (!nodes.element(n)) {
    nodes.insert(n);
    for (const auto& head : heads(n)) {
      reachables(head, nodes);
    }
  }
}
void Graph::reachables(const llvm::Value& v, NodeSet& nodes) const {
  for (const auto& tail : heads(Node::from_value(v))) {
    reachables(tail, nodes);
  }
}
auto Graph::insert(iterator hint, const Edge& e) -> Result {
  const auto [lb, ub] = std::equal_range(hint, end(), e);
  if (lb == ub) {
    return {Super::insert(lb, e), true};
  } else {
    return {lb, false};
  }
}

}  // namespace stacksafe
