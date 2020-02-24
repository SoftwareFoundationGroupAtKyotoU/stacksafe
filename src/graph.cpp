#include "graph.hpp"
#include <algorithm>

namespace stacksafe {

bool Graph::append(const Node& from, const Node& to) {
  return std::get<1>(insert(begin(), Edge{from, to}));
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
