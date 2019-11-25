#include "env.hpp"
#include <algorithm>
#include "domain.hpp"
#include "pool.hpp"

namespace stacksafe {

bool Env::includes(const Env& env) const {
  return Zdd::includes(zdd_, env.zdd_);
  const auto pred = [& self = *this](const value_type& pair) {
    return self.includes(pair.second);
  };
  return (filter_.includes(env.filter_) &&
          std::all_of(env.begin(), env.end(), pred));
}
void Env::merge(const Env& env) {
  Zdd::Pairs pairs;
  for (const auto& [key, ref] : env) {
    insert(key, ref);
    for (const auto& [k, val] : ref.get()) {
      pairs.emplace(k, val);
    }
  }
  zdd_ = Zdd::merge(zdd_, Zdd::make(pairs));
  filter_.merge(env.filter_);
}
Env::Env(std::size_t count) : filter_{count} {}
void Env::insert(const MapRef& ref) {
  for (const auto& key : Map::keys(ref.get())) {
    insert(key, ref);
  }
  filter_.merge(ref.get().filter());
}
Domain Env::lookup(const Value& key) const {
  Domain dom;
  const auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.merge(it->second.get().lookup(key));
  }
  return dom;
}
void Env::update(Map& map, const Value& key, const Domain& dom) const {
  const auto [lb, ub] = Super::equal_range(key);
  for (const auto& val : dom) {
    if (!range_contains(lb, ub, val)) {
      map.insert(key, val);
    }
  }
}
void Env::insert(const Value& key, const MapRef& ref) {
  const auto [lb, ub] = Super::equal_range(key);
  if (!range_contains(lb, ub, ref)) {
    Super::emplace_hint(lb, key, ref);
  }
}
bool Env::includes(const MapRef& ref) const {
  const auto pred = [& self = *this](const Map::value_type& pair) {
    return self.contains(pair.first, pair.second);
  };
  return std::all_of(ref.get().begin(), ref.get().end(), pred);
}
bool Env::contains(const Value& key, const Value& val) const {
  const auto [lb, ub] = Super::equal_range(key);
  return range_contains(lb, ub, val);
}
bool Env::range_contains(const_iterator lb, const_iterator ub,
                         const Value& val) {
  const auto pred = [&val](const value_type& pair) {
    return pair.second.get().contains(pair.first, val);
  };
  return std::any_of(lb, ub, pred);
}
bool Env::range_contains(const_iterator lb, const_iterator ub,
                         const MapRef& ref) {
  const auto pred = [&ref](const value_type& pair) {
    return &pair.second.get() == &ref.get();
  };
  return std::any_of(lb, ub, pred);
}

MutableEnv::MutableEnv(const Env& env, const MapPool& pool)
    : Env{env}, diff_{pool.make_map()} {}
const Env& MutableEnv::finish(MapPool& pool) {
  if (!diff_.empty()) {
    Env::insert(pool.add(diff_));
  }
  return *this;
}
void MutableEnv::insert(const Value& key, const Domain& dom) {
  Env::update(diff_, key, dom);
}
Domain MutableEnv::lookup(const Value& key) const {
  Domain dom = Env::lookup(key);
  dom.merge(diff_.lookup(key));
  return dom;
}

}  // namespace stacksafe
