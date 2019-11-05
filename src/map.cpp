#include "map.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

void Map::insert(const Register &key, const Domain &val) {
  if (auto it = Super::find(key.value()); it != end()) {
    it->second.merge(val);
  } else {
    Super::try_emplace(key.value(), val);
  }
}
const Domain &Map::lookup(const Register &key) const {
  if (auto it = Super::find(key.value()); it != end()) {
    return it->second;
  }
  return Domain::get_empty();
}
void Map::merge(const Map &that) {
  for (const auto &[k, v] : that) {
    insert(k ? Register::get_local(*k) : Register::get_global(), v);
  }
}
bool Map::includes(const Map &that) const {
  for (const auto &[key, rhs] : that) {
    if (auto lhs = Super::find(key);
        lhs != end() && lhs->second.includes(rhs)) {
      continue;
    }
    return false;
  }
  return true;
}
void to_json(Json &j, const Map &x) {
  static const std::string global{"@"};
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[key ? get_operand(*key) : global] = val;
  }
  j = obj;
}

}  // namespace stacksafe
