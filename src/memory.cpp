#include "memory.hpp"

namespace stacksafe {

void to_json(Json& j, const Heap& x) {
  Json tmp;
  for (auto& [k, v] : x) {
    tmp[k.to_str()] = v;
  }
  j = tmp;
}

}  // namespace stacksafe
