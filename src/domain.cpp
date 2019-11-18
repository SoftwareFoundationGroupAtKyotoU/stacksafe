#include "domain.hpp"

namespace stacksafe {

void Domain::insert(const Symbol &sym) {
  Super::insert(sym);
}
void Domain::merge(const Domain &that) {
  for (const auto &sym : that) {
    insert(sym);
  }
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
      ret.insert(sym);
    }
  }
  return ret;
}
}  // namespace stacksafe
