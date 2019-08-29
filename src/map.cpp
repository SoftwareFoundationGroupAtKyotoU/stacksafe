#include "map.hpp"
#include "json.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

template <typename Key>
auto Map<Key>::get(const Key& key) -> Val* {
  const auto& self = *this;
  return const_cast<Val*>(self.get(key));
}
template <typename Key>
auto Map<Key>::get(const Key& key) const -> const Val* {
  if (auto it = Base::find(key); it != end()) {
    return &it->second;
  }
  return nullptr;
}
template <typename Key>
void Map<Key>::insert(const Key& key, const Symbol& sym) {
  Val val;
  val.insert(sym);
  insert(key, val);
}
template <typename Key>
void Map<Key>::insert(const Key& key, const Val& val) {
  if (auto p = get(key)) {
    p->insert(val);
  } else {
    Base::try_emplace(key, val);
  }
}
template <typename Key>
void to_json(Json& j, const Map<Key>& x) {
  Json obj;
  for (auto& [key, val] : x) {
    Json tmp = key;
    if (tmp.is_string()) {
      auto k = tmp.get<std::string>();
      obj[k] = val;
    }
  }
  j = obj;
}

template class Map<Register>;
template class Map<Symbol>;

template void to_json<Register>(Json&, const Stack&);
template void to_json<Symbol>(Json&, const Heap&);

}  // namespace stacksafe
