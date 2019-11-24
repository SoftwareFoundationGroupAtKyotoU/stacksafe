#include "env.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "domain.hpp"
#include "pool.hpp"

namespace stacksafe {
namespace {
bool range_contains(Env::const_iterator lb, Env::const_iterator ub,
                    const Value& val) {
  const auto pred = [&val](const Env::value_type& pair) {
    return pair.second.get().element(pair.first, val);
  };
  return std::any_of(lb, ub, pred);
}
bool range_contains(Env::const_iterator lb, Env::const_iterator ub,
                    const MapRef& ref) {
  const auto pred = [&ref](const Env::value_type& pair) {
    return &pair.second.get() == &ref.get();
  };
  return std::any_of(lb, ub, pred);
}
}  // namespace

bool Env::includes(const Env& env) const {
  const auto pred = [& self = *this](const value_type& pair) {
    return self.includes(pair.second);
  };
  return std::all_of(env.begin(), env.end(), pred);
}
void Env::merge(const Env& env) {
  for (const auto& [key, ref] : env) {
    insert(key, ref);
  }
}
void Env::insert(const MapRef& ref) {
  for (const auto& key : Map::keys(ref.get())) {
    insert(key, ref);
  }
}
void Env::insert(Map& map, const Value& key, const Domain& dom) {
  const auto [lb, ub] = Super::equal_range(key);
  for (const auto& val : dom) {
    if (!range_contains(lb, ub, val)) {
      map.insert(key, val);
    }
  }
}
Domain Env::lookup(const Value& key) const {
  Domain dom;
  const auto [lb, ub] = Env::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.merge(it->second.get().lookup(key));
  }
  return dom;
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

MutableEnv::MutableEnv(const Env& env) : Env{env} {}
MutableEnv::MutableEnv(const llvm::Function& f) {
  const auto g = Value::get_symbol();
  diff_.insert(g, g);
  for (const auto& a : f.args()) {
    const auto arg = Value::get_symbol(a);
    diff_.insert(arg, arg);
    diff_.insert(Value::get_register(a), arg);
  }
}
const Env& MutableEnv::finish(MapPool& pool) {
  if (!diff_.empty()) {
    Env::insert(pool.add(diff_));
  }
  return *this;
}
void MutableEnv::insert(const Value& key, const Domain& dom) {
  Env::insert(diff_, key, dom);
}
Domain MutableEnv::lookup(const Value& key) const {
  Domain dom = Env::lookup(key);
  dom.merge(diff_.lookup(key));
  return dom;
}

}  // namespace stacksafe
