#include "domain.hpp"

namespace stacksafe {

void Domain::insert(const Symbol& sym) { Base::insert(sym); }
void Domain::insert(const Domain& dom) { Base::insert(dom.begin(), dom.end()); }
void to_json(Json& j, const Domain& x) {
  Json tmp;
  for (auto& e : x) {
    tmp.push_back(e);
  }
  j = tmp;
}

}  // namespace stacksafe
