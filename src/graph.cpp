#include "graph.hpp"

namespace stacksafe {

void Components::transfer(const Blocks& b, const Map& pred) {
  for (const auto& succ : b.successors()) {
    find(succ).merge(pred);
  }
}
Map& Components::find(BB b) {
  const auto pred = [b](const Pair& pair) {
    return std::get<0>(pair).contains(b);
  };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return std::get<1>(*it);
}

}  // namespace stacksafe
