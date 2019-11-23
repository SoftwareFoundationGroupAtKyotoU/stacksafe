#ifndef INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
#define INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E

#include <unordered_map>
#include "map.hpp"
#include "value.hpp"

namespace stacksafe {

class Env : private std::unordered_multimap<Value, MapRef> {
  using Super = std::unordered_multimap<Value, MapRef>;
  using Super::const_iterator;

 protected:
  using Super::equal_range, Super::emplace_hint;

 public:
  bool contains(const Value& key, const Value& val) const;
  bool includes(const MapRef& ref) const;
  bool includes(const Env& env) const;
  void merge(const Env& env);
  static bool range_contains(const_iterator lb, const_iterator ub,
                             const Value& val);
  static bool range_contains(const_iterator lb, const_iterator ub,
                             const MapRef& ref);
};

class MutableEnv : private Env {
  MapRef ref_;

 public:
  MutableEnv(const Env& env, const MapRef& ref);
  void insert(const MapRef& ref);
  void insert(const Value& key, const Domain& dom);
  Domain lookup(const Value& key) const;
  const Env& env() const;
};

}  // namespace stacksafe
#endif  // INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
