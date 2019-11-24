#ifndef INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
#define INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E

#include <unordered_map>
#include "filter.hpp"
#include "map.hpp"
#include "value.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {
class MapPool;

class Env : private std::unordered_multimap<Value, MapRef> {
  friend class MapPool;
  using Super = std::unordered_multimap<Value, MapRef>;
  BloomFilter filter_;

 public:
  bool includes(const Env& env) const;
  void merge(const Env& env);

 protected:
  explicit Env(std::size_t count);
  void insert(const MapRef& ref);
  void insert(Map& map, const Value& key, const Domain& dom);
  Domain lookup(const Value& key) const;

 private:
  void insert(const Value& key, const MapRef& ref);
  bool includes(const MapRef& ref) const;
  bool contains(const Value& key, const Value& val) const;
  static bool range_contains(const_iterator lb, const_iterator ub,
                             const Value& val);
  static bool range_contains(const_iterator lb, const_iterator ub,
                             const MapRef& ref);
};

class MutableEnv : private Env {
  Map diff_;

 public:
  explicit MutableEnv(const Env& env);
  const Env& finish(MapPool& pool);
  void insert(const Value& key, const Domain& dom);
  Domain lookup(const Value& key) const;
};

}  // namespace stacksafe
#endif  // INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
