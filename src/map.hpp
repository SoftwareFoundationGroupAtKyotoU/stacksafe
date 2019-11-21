#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "symbol.hpp"
#include "value.hpp"

namespace llvm {
class Function;
class Instruction;
}  // namespace llvm

namespace stacksafe {
class Domain;

class Map : private std::unordered_multimap<Symbol, Symbol> {
  using Super = std::unordered_multimap<Symbol, Symbol>;
  void insert(const Symbol &key, const Symbol &val);

 public:
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Symbol &key) const;
  bool includes(const Map &map) const;
  void merge(const Map &map);
  static Map init(const llvm::Function &f);
  static bool equals(const Map &lhs, const Map &rhs);
  static Domain keys(const Map &map);
  friend llvm::hash_code hash_value(const Map &map);
};
llvm::hash_code hash_value(const Map &map);

class MapRef {
  const Map *ptr_;

 public:
  const std::size_t hash;
  explicit MapRef(const Map &map);
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
