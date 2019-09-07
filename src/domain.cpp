#include "domain.hpp"
#include <utility>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

void Domain::insert(const Symbol& sym) { Base::insert(sym); }
void Domain::insert(const Domain& dom) { Base::insert(dom.begin(), dom.end()); }
bool Domain::includes(const Symbol& sym) const { return 0 < Base::count(sym); }
bool Domain::includes(const Domain& that) const {
  return std::includes(begin(), end(), that.begin(), that.end());
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
