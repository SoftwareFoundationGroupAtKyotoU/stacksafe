#include "domain.hpp"
#include <algorithm>

namespace stacksafe {

Domain::Domain(const Value &val) : Super{val} {}
bool Domain::insert(const Value &val) {
  const auto [lb, ub] = std::equal_range(begin(), end(), val);
  if (lb == ub) {
    Super::insert(lb, val);
    return true;
  }
  return false;
}
bool Domain::merge(const Domain &dom) {
  const auto size = Super::size();
  for (const auto &val : dom) {
    insert(val);
  }
  return size != Super::size();
}
bool Domain::element(const Value &val) const {
  return std::binary_search(begin(), end(), val);
}
bool Domain::has_local() const {
  const auto pred = [](const Value &val) { return val.is_local(); };
  return std::any_of(begin(), end(), pred);
}

}  // namespace stacksafe
