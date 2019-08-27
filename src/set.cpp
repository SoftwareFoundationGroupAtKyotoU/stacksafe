#include "set.hpp"

namespace stacksafe {

void to_json(Json& j, const Domain& x) {
  Json tmp;
  for (auto& e : x) {
    tmp.push_back(e);
  }
  j = tmp;
}

}  // namespace stacksafe
