#include "env.hpp"
#include <algorithm>
#include "domain.hpp"
#include "pool.hpp"

namespace stacksafe {

bool Env::contains(const Value& key, const Value& val) const {
  const auto [lb, ub] = Super::equal_range(key);
  return range_contains(lb, ub, val);
}
bool Env::includes(const MapRef& ref) const {
  const auto pred = [& self = *this](const auto& pair) {
    return self.contains(pair.first, pair.second);
  };
  return std::all_of(ref.get().begin(), ref.get().end(), pred);
}
bool Env::includes(const Env& env) const {
  const auto pred = [& self = *this](const auto& pair) {
    return self.includes(pair.second);
  };
  return std::all_of(env.begin(), env.end(), pred);
}
void Env::merge(const Env& env) {
  for (const auto& [key, ref] : env) {
    const auto [lb, ub] = Super::equal_range(key);
    if (!range_contains(lb, ub, ref)) {
      Super::emplace_hint(lb, key, ref);
    }
  }
}
bool Env::range_contains(const_iterator lb, const_iterator ub,
                         const Value& val) {
  const auto pred = [&val](const auto& pair) {
    return pair.second.get().element(pair.first, val);
  };
  return std::any_of(lb, ub, pred);
}
bool Env::range_contains(const_iterator lb, const_iterator ub,
                         const MapRef& ref) {
  const auto pred = [&ref](const auto& pair) {
    return &pair.second.get() == &ref.get();
  };
  return std::any_of(lb, ub, pred);
}

MutableEnv::MutableEnv(const Env& env, const MapRef& ref)
    : Env{env}, ref_{ref} {}
void MutableEnv::finish(MapPool& pool) {
  for (const auto& [key, ref] : *this) {
    if (&ref.get() == &ref_.get()) {
      return;
    }
  }
  pool.remove(ref_);
}
void MutableEnv::insert(const MapRef& ref) {
  for (const auto& [key, val] : ref.get()) {
    Domain dom;
    dom.insert(val);
    insert(key, dom);
  }
}
void MutableEnv::insert(const Value& key, const Domain& dom) {
  const auto [lb, ub] = Env::equal_range(key);
  for (const auto& val : dom) {
    if (!Env::range_contains(lb, ub, val)) {
      ref_.get().insert(key, val);
      Env::emplace_hint(lb, key, ref_);
    }
  }
}
Domain MutableEnv::lookup(const Value& key) const {
  Domain dom;
  const auto [lb, ub] = Env::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.merge(it->second.get().lookup(key));
  }
  return dom;
}
const Env& MutableEnv::env() const {
  return *this;
}

}  // namespace stacksafe
