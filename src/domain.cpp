#include "domain.hpp"
#include <utility>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

Domain::Domain(std::initializer_list<Symbol> list) : Super{list} {}
bool Domain::merge(const Domain& that) {
  if (includes(that)) {
    return false;
  } else {
    Super::insert(that.begin(), that.end());
    return true;
  }
}
bool Domain::includes(const Domain& that) const {
  return std::includes(begin(), end(), that.begin(), that.end());
}
bool Domain::has_local() const {
  for (const auto& sym : *this) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}
void to_json(Json& j, const Domain& x) {
  Json::array_t arr;
  for (const auto& e : x) {
    arr.push_back(e.repr());
  }
  j = arr;
}
Fabric dump(const Domain& domain) {
  Fabric ret;
  bool first = true;
  for (const auto& symbol : domain) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(dump(symbol));
  }
  return ret.quote("[", "]");
}

}  // namespace stacksafe
