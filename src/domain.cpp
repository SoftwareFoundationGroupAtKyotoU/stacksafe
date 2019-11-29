#include "domain.hpp"
#include <algorithm>
#include "value.hpp"

namespace stacksafe {
namespace {
bool compare(const Symbol &lhs, const Symbol &rhs) {
  return lhs.sym() < rhs.sym();
}
bool is_local(const Symbol &sym) {
  return sym.is_local();
}
}  // namespace

Domain::Domain(const Value &val) {
  insert(val);
}
bool Domain::insert(const Value &val) {
  if (auto sym = val.as_symbol()) {
    const auto [lb, ub] = std::equal_range(begin(), end(), *sym, compare);
    if (lb == ub) {
      Super::insert(lb, *sym);
      return true;
    }
  }
  return false;
}
bool Domain::merge(const Domain &dom) {
  const auto size = Super::size();
  for (const auto &sym : dom) {
    insert(sym);
  }
  return size != Super::size();
}
bool Domain::element(const Symbol &sym) const {
  return std::binary_search(begin(), end(), sym, compare);
}
bool Domain::has_local() const {
  return std::any_of(begin(), end(), is_local);
}

}  // namespace stacksafe
