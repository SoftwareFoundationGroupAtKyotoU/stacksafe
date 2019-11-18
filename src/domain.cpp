#include "domain.hpp"

namespace stacksafe {

void Domain::merge(const Domain &that) {
  Super::insert(that.begin(), that.end());
}
bool Domain::includes(const Domain &that) const {
  for (const auto &sym : that) {
    if (Super::count(sym) == 0) {
      return false;
    }
  }
  return true;
}
Domain Domain::minus(const Domain &that) const {
  Domain ret;
  for (const auto &sym : *this) {
    if (that.Super::count(sym) == 0) {
      ret.Super::insert(sym);
    }
  }
  return ret;
}
Domain Domain::get_empty() {
  static const Domain dom{};
  return dom;
}
Domain Domain::get_global() {
  static const auto dom = get_singleton(Symbol::get_global());
  return dom;
}
Domain Domain::get_singleton(const Symbol &sym) {
  auto dom = get_empty();
  dom.Super::insert(sym);
  return dom;
}
}  // namespace stacksafe
