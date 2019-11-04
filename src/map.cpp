#include "map.hpp"
#include "json.hpp"

namespace stacksafe {

void Map::insert(const Register &key, const Domain &val) {
  if (auto it = Super::find(key); it != end()) {
    it->second.merge(val);
  } else {
    Super::try_emplace(key, val);
  }
}
const Domain &Map::lookup(const Register &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  }
  return Domain::get_empty();
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
void to_json(Json &j, const Map &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

void Heap::insert(const Register &key, const Domain &val) {
  Super::insert(key, val);
}
const Domain &Heap::lookup(const Register &key) const {
  return Super::lookup(key);
}
void Heap::merge(const Heap &that) {
  Super::merge(that);
}
bool Heap::includes(const Heap &that) const {
  return Super::includes(that);
}
void to_json(Json &j, const Heap &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

void Stack::insert(const Register &key, const Domain &val) {
  Super::insert(key, val);
}
const Domain &Stack::lookup(const Register &key) const {
  return Super::lookup(key);
}
void Stack::merge(const Stack &that) {
  Super::merge(that);
}
bool Stack::includes(const Stack &that) const {
  return Super::includes(that);
}
void to_json(Json &j, const Stack &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

}  // namespace stacksafe
