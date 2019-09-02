#include "map.hpp"
#include "json.hpp"

namespace stacksafe {

template <typename K>
bool Map<K>::exists(const K& key) const {
  return 0 < Base::count(key);
}
template <typename K>
auto Map<K>::get(const K& key) -> Domain* {
  const auto& self = *this;
  return const_cast<Domain*>(self.get(key));
}
template <typename K>
auto Map<K>::get(const K& key) const -> const Domain* {
  if (auto it = Base::find(key); it != end()) {
    return &it->second;
  }
  return nullptr;
}
template <typename K>
void Map<K>::insert(const K& key, const Symbol& sym) {
  Domain val;
  val.insert(sym);
  insert(key, val);
}
template <typename K>
void Map<K>::insert(const K& key, const Domain& val) {
  if (auto p = get(key)) {
    p->insert(val);
  } else {
    Base::try_emplace(key, val);
  }
}
template <typename K>
void Map<K>::insert(const K& key) {
  if (!exists(key)) {
    Base::try_emplace(key, Domain{});
  }
}
template <typename K>
void Map<K>::insert(const Map& that) {
  for (auto& [k, v] : that) {
    insert(k, v);
  }
}
template <typename K>
bool Map<K>::includes(const Map& that) const {
  for (auto& [key, thisv] : *this) {
    if (auto thatv = that.get(key); thatv && thisv.includes(*thatv)) {
      continue;
    }
    return false;
  }
  return true;
}
template <typename K>
void to_json(Json& j, const Map<K>& x) {
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

template class Map<Value>;
template class Map<Symbol>;

template void to_json<Value>(Json&, const Stack&);
template void to_json<Symbol>(Json&, const Heap&);

}  // namespace stacksafe
