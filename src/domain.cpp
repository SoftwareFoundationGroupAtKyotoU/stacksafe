#include "domain.hpp"
#include <utility>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

Domain::Domain(std::initializer_list<Symbol> list) : Super{list} {}
void Domain::insert(const Domain& dom) {
  Super::insert(dom.begin(), dom.end());
}
bool Domain::includes(const Symbol& sym) const {
  return 0 < Super::count(sym);
}
bool Domain::includes(const Domain& that) const {
  return std::includes(begin(), end(), that.begin(), that.end());
}
bool Domain::has_local() const {
  for (auto& sym : *this) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}
void to_json(Json& j, const Domain& x) {
  Json::array_t arr;
  for (auto& e : x) {
    arr.push_back(e);
  }
  j = arr;
}
Fabric dump(const Domain& domain) {
  Fabric ret;
  bool first = true;
  for (auto& symbol : domain) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(dump(symbol));
  }
  return ret.quote("[", "]");
}

}  // namespace stacksafe
