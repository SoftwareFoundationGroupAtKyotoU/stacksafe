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
bool Map<K>::insert(const K& key, const Domain& val) {
  if (auto it = Super::find(key); it != end()) {
    return it->second.merge(val);
  } else {
    Super::try_emplace(key, val);
    return true;
  }
}
template <typename K>
bool Map<K>::merge(const Map& that) {
  bool ret = false;
  for (const auto& [k, v] : that) {
    if (insert(k, v)) {
      ret = true;
    }
  }
  return ret;
}
template <typename K>
bool Map<K>::includes(const Map& that) const {
  for (const auto& [key, rhs] : that) {
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
Fabric Map<K>::diff(const Map& that) const {
  Fabric ret;
  for (const auto& [key, rhs] : that) {
    if (auto it = Super::find(key); it != end()) {
      const auto& lhs = it->second;
      assert(rhs.includes(lhs));
      if (lhs.includes(rhs)) {
        ret.append(" ").append(dump(key)).append(": ").append(dump(lhs)).next();
      } else {
        ret.append("-").append(dump(key)).append(": ").append(dump(lhs)).next();
        ret.append("+").append(dump(key)).append(": ").append(dump(rhs)).next();
      }
    }
  }
  return ret;
}
template <typename K>
void to_json(Json& j, const Map<K>& x) {
  Json::object_t obj;
  for (const auto& [key, val] : x) {
    obj[key.repr()] = val;
  }
  j = obj;
}
template <typename K>
Fabric dump(const Map<K>& map) {
  Fabric ret, tmp;
  bool first = true;
  for (const auto& [key, value] : map) {
    if (0 < value.size()) {
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
