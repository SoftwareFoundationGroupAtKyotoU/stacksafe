#include "domain.hpp"
#include <algorithm>

namespace stacksafe {

Domain::Domain(const Value &val) : Super{val} {}
bool Domain::insert(const Value &sym) {
  const auto [lb, ub] = std::equal_range(begin(), end(), sym);
  if (lb == ub) {
    Super::insert(lb, sym);
    return true;
  }
  return false;
}
bool Domain::merge(const Domain &that) {
  const auto size = Super::size();
  for (const auto &sym : that) {
    insert(sym);
  }
  return size != Super::size();
}
bool Domain::element(const Value &sym) const {
  return std::binary_search(begin(), end(), sym);
}
bool Domain::has_local() const {
  const auto pred = [](const auto &sym) { return sym.is_local(); };
  return std::any_of(begin(), end(), pred);
}

}  // namespace stacksafe
