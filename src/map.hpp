#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <unordered_map>
#include "hash.hpp"
#include "value.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Domain;

class Map : private std::unordered_multimap<Value, Value> {
  using Super = std::unordered_multimap<Value, Value>;
  std::size_t hash_;

 public:
  using Super::begin, Super::end, Super::empty;
  using Super::value_type;
  Map();
  explicit Map(const llvm::Function &f);
  void insert(const Value &key, const Value &val);
  void insert(const Value &key, const Domain &val);
  Domain lookup(const Value &key) const;
  bool element(const Value &key, const Value &val) const;
  bool includes(const Map &map) const;
  void merge(const Map &map);
  static bool equals(const Map &lhs, const Map &rhs);
  static Domain keys(const Map &map);
  friend llvm::hash_code hash_value(const Map &map);
};

class MapRef {
  Map *ptr_;

 public:
  explicit MapRef(Map &map);
  const Map &get() const;
  Map &get();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
