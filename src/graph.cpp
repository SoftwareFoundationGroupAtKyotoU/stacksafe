#include "graph.hpp"
#include <algorithm>

namespace stacksafe {
namespace {
bool tail_cmp(const Edge& lhs, const Edge& rhs) {
  return lhs.tail() < rhs.tail();
}
}  // namespace

bool Graph::append(const Node& tail, const Node& head) {
  return std::get<1>(insert(begin(), Edge{tail, head}));
}
NodeSet Graph::outedges(const Node& tail) const {
  NodeSet nodes;
  const auto [lb, ub] = std::equal_range(begin(), end(), Edge{tail}, tail_cmp);
  nodes.reserve(std::distance(lb, ub));
  for (auto it = lb; it != ub; ++it) {
    nodes.push_back(it->head());
  }
  return nodes;
}
NodeSet Graph::tails() const {
  NodeSet nodes;
  for (const auto& e : *this) {
    nodes.push_back(e.tail());
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
auto Graph::insert(iterator hint, const Edge& e) -> Result {
  const auto [lb, ub] = std::equal_range(hint, end(), e);
  if (lb == ub) {
    return {Super::insert(lb, e), true};
  } else {
    return {lb, false};
  }
}

}  // namespace stacksafe
