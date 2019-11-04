#include "domain.hpp"
#include <algorithm>
#include <iterator>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

Domain::Domain(std::initializer_list<Symbol> list) : Super{list} {}
void Domain::merge(const Domain &that) {
  Super::insert(that.begin(), that.end());
}
bool Domain::includes(const Domain &that) const {
  return std::includes(begin(), end(), that.begin(), that.end());
}
Domain Domain::minus(const Domain &that) const {
  Domain ret;
  auto inserter = std::inserter(static_cast<Super &>(ret), ret.end());
  std::set_difference(begin(), end(), that.begin(), that.end(), inserter);
  return ret;
}
bool Domain::has_local() const {
  for (const auto &sym : *this) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}
const Domain &Domain::get_global() {
  static const Domain dom{Symbol::global()};
  return dom;
}
Domain Domain::global() {
  return Domain{Symbol::global()};
}
void to_json(Json &j, const Domain &x) {
  Json::array_t arr;
  for (const auto &e : x) {
    arr.push_back(to_str(e));
  }
  j = arr;
}
Fabric dump(const Domain &domain) {
  Fabric ret;
  bool first = true;
  for (const auto &symbol : domain) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(to_str(symbol));
  }
  return ret.quote("[", "]");
}

}  // namespace stacksafe
