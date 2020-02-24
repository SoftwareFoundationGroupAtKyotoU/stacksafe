#include "graph.hpp"
#include <algorithm>

namespace stacksafe {

bool Graph::append(const Node& from, const Node& to) {
  return std::get<1>(insert(begin(), Edge{from, to}));
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
