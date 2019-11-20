#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "symbol.hpp"
#include "value.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {
class Domain;

class Map : private std::unordered_multimap<Value, Symbol> {
  using Super = std::unordered_multimap<Value, Symbol>;

 public:
  void insert(const Value &key, const Symbol &val);
  void insert(const Value &key, const Domain &val);
  void insert(const Symbol &key, const Symbol &val);
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Value &key) const;
  Domain lookup(const Symbol &key) const;
  bool includes(const Map &map) const;
  void merge(const Map &map);
  static Map init_heap(const llvm::Function &f);
  static Map init_stack(const llvm::Function &f);
  static bool equals(const Map &lhs, const Map &rhs);
  static std::unordered_set<Value> keys(const Map &map);
  static std::size_t hash(const Map &map);
};

class MapRef {
  const Map *flat_;

 public:
  explicit MapRef(const Map &flat);
  const Map &get() const;
};
bool operator==(const MapRef &lhs, const MapRef &rhs);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapRef> {
  size_t operator()(const stacksafe::MapRef &f) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
