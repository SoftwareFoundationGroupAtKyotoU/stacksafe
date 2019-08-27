#include "memory.hpp"

namespace stacksafe {

void to_json(Json& j, const Stack& x) {
  Json tmp;
  for (auto& [k, v] : x) {
    tmp[k.to_str()] = v;
  }
  j = tmp;
}

void Heap::insert(const Key& key, const Symbol& val) {
  if (auto it = Base::find(key); it != end()) {
    it->second.insert(val);
  } else {
    Base::try_emplace(key, std::set{val});
  }
}
void to_json(Json& j, const Heap& x) {
  Json tmp;
  for (auto& [k, v] : x) {
    tmp[k.to_str()] = v;
  }
  j = tmp;
}

}  // namespace stacksafe
