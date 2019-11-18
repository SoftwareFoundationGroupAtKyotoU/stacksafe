#include "map.hpp"
#include <llvm/ADT/Hashing.h>

namespace stacksafe {

void Map::insert(const Value &key, const Domain &val) {
  if (auto it = Super::find(key); it != end()) {
    it->second.merge(val);
  } else {
    Super::try_emplace(key, val);
  }
}
Domain Map::lookup(const Value &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  }
  return Domain{};
}
void Map::merge(const Map &that) {
  for (const auto &[k, v] : that) {
    insert(k, v);
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
std::size_t Map::hash(const Map &m) {
  std::size_t h = 0;
  for (const auto &[val, dom] : m) {
    for (const auto &sym : dom) {
      h ^= llvm::hash_combine(Value::hash(val), Value::hash(sym.value()));
    }
  }
  return h;
}

}  // namespace stacksafe
