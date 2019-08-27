#include "memory.hpp"

namespace stacksafe {

auto Stack::get(const Key& key) -> Value* {
  const auto& self = *this;
  return const_cast<Value*>(self.get(key));
}
auto Stack::get(const Key& key) const -> const Value* {
  if (auto it = Base::find(key); it != end()) {
    return &it->second;
  }
  return nullptr;
}
void Stack::insert(const Key& key, const Symbol& val) {
  if (auto p = get(key)) {
    p->insert(val);
  } else {
    Base::try_emplace(key, Value{val});
  }
}
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
