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
bool Domain::element(const Symbol &sym) const {
  return Super::count(sym) != 0;
}
bool Domain::includes(const Domain &that) const {
  for (const auto &sym : that) {
    if (!element(sym)) {
      return false;
    }
  }
  return true;
}
Domain Domain::minus(const Domain &that) const {
  Domain ret;
  for (const auto &sym : *this) {
    if (!that.element(sym)) {
      ret.insert(sym);
    }
  }
  return ret;
}
}  // namespace stacksafe
