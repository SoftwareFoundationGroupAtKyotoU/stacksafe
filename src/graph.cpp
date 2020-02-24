#include "graph.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {
namespace {
bool tail_cmp(const Edge& lhs, const Edge& rhs) {
  return lhs.tail() < rhs.tail();
}
}  // namespace

void Graph::init(const llvm::Function& f) {
  const Node g{Symbol::get_global()};
  append(g, g);
  for (const auto& a : f.args()) {
    const Node sym{Symbol{a}};
    const Node reg{Register{a}};
    append(sym, sym);
    append(reg, sym);
  }
}
bool Graph::append(const Node& tail, const Node& head) {
  return std::get<1>(insert(begin(), Edge{tail, head}));
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
auto Graph::insert(iterator hint, const Edge& e) -> Result {
  const auto [lb, ub] = std::equal_range(hint, end(), e);
  if (lb == ub) {
    return {Super::insert(lb, e), true};
  } else {
    return {lb, false};
  }
}

}  // namespace stacksafe
