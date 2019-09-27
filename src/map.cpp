#include "map.hpp"
#include <utility>
#include "fabric.hpp"
#include "json.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

template <typename K>
std::optional<Domain> Map<K>::get(const K& key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  }
  return std::nullopt;
}
template <typename K>
void Map<K>::insert(const K& key, const Domain& val) {
  if (auto it = Super::find(key); it != end()) {
    it->second.insert(val);
  } else {
    Super::try_emplace(key, val);
  }
}
template <typename K>
void Map<K>::merge(const Map& that) {
  for (auto& [k, v] : that) {
    insert(k, v);
  }
}
template <typename K>
bool Map<K>::includes(const Map& that) const {
  for (auto& [key, rhs] : that) {
    if (auto lhs = get(key); lhs && lhs->includes(rhs)) {
      continue;
    }
    return false;
  }
  return true;
}
template <typename K>
Domain Map<K>::lookup(const K& key) const {
  if (auto dom = get(key)) {
    return *dom;
  }
  return Domain{};
}
template <typename K>
void to_json(Json& j, const Map<K>& x) {
  Json::object_t obj;
  for (auto& [key, val] : x) {
    obj[key.repr()] = val;
  }
  j = obj;
}
template <typename K>
Fabric dump(const Map<K>& map) {
  Fabric ret, tmp;
  bool first = true;
  for (auto& [key, value] : map) {
    if (!std::exchange(first, false)) {
      tmp.append(",").next();
    }
    tmp.append(dump(key)).append(":");
    if (1 < value.size()) {
      tmp.next();
      tmp.append(dump(value).indent(2));
    } else {
      tmp.append(" ").append(dump(value));
    }
  }
  ret.append("{").next();
  ret.append(tmp.indent(2)).next();
  return ret.append("}");
}

template class Map<Register>;
template class Map<Symbol>;
template void to_json<Register>(Json&, const Map<Register>&);
template void to_json<Symbol>(Json&, const Map<Symbol>&);
template Fabric dump<Register>(const Map<Register>&);
template Fabric dump<Symbol>(const Map<Symbol>&);

}  // namespace stacksafe
