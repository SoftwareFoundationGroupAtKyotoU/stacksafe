#include "domain.hpp"
#include <algorithm>

namespace stacksafe {

void Domain::insert(const Symbol &sym) {
  const auto [lb, ub] = std::equal_range(begin(), end(), sym);
  if (lb == ub) {
    Super::insert(lb, sym);
  }
}
void Domain::merge(const Domain &that) {
  for (const auto &sym : that) {
    insert(sym);
  }
}
bool Domain::element(const Symbol &sym) const {
  return std::binary_search(begin(), end(), sym);
}

}  // namespace stacksafe
