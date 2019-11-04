#include "map.hpp"
#include <utility>
#include "fabric.hpp"
#include "json.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

void Map::insert(const Number &key, const Domain &val) {
  if (auto it = Super::find(key); it != end()) {
    it->second.merge(val);
  } else {
    Super::try_emplace(key, val);
  }
}
Domain Map::lookup(const Number &key) const {
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
Fabric Map::diff(const Map &that) const {
  Fabric ret;
  for (const auto &[key, rhs] : that) {
    if (auto it = Super::find(key); it != end()) {
      const auto &lhs = it->second;
      assert(rhs.includes(lhs));
      if (lhs.includes(rhs)) {
        if (!lhs.empty()) {
          ret.append(" ")
              .append(to_str(key))
              .append(": ")
              .append(dump(lhs))
              .next();
        }
      } else {
        ret.append("-")
            .append(to_str(key))
            .append(": ")
            .append(dump(lhs))
            .next();
        ret.append("+")
            .append(to_str(key))
            .append(": ")
            .append(dump(rhs))
            .next();
      }
    }
  }
  return ret;
}
void to_json(Json &j, const Map &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

void Heap::insert(const Symbol &key, const Domain &val) {
  Super::insert(key.number(), val);
}
Domain Heap::lookup(const Symbol &key) const {
  return Super::lookup(key.number());
}
void Heap::merge(const Heap &that) {
  Super::merge(that);
}
bool Heap::includes(const Heap &that) const {
  return Super::includes(that);
}
Fabric Heap::diff(const Heap &that) const {
  return Super::diff(that);
}
void to_json(Json &j, const Heap &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

void Stack::insert(const Register &key, const Domain &val) {
  Super::insert(key.number(), val);
}
Domain Stack::lookup(const Register &key) const {
  return Super::lookup(key.number());
}
void Stack::merge(const Stack &that) {
  Super::merge(that);
}
bool Stack::includes(const Stack &that) const {
  return Super::includes(that);
}
Fabric Stack::diff(const Stack &that) const {
  return Super::diff(that);
}
void to_json(Json &j, const Stack &x) {
  Json::object_t obj;
  for (const auto &[key, val] : x) {
    obj[to_str(key)] = val;
  }
  j = obj;
}

}  // namespace stacksafe
