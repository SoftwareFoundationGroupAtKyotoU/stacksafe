#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <unordered_map>
#include "hash.hpp"
#include "symbol.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Domain;

class Map : private std::unordered_multimap<Symbol, Symbol> {
  using Super = std::unordered_multimap<Symbol, Symbol>;
  std::size_t hash_;
  void insert(const Symbol &key, const Symbol &val);

 public:
  Map();
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Symbol &key) const;
  bool includes(const Map &map) const;
  void merge(const Map &map);
  static Map init(const llvm::Function &f);
  static bool equals(const Map &lhs, const Map &rhs);
  static Domain keys(const Map &map);
  friend llvm::hash_code hash_value(const Map &map);
};

class MapRef {
  const Map *ptr_;

 public:
  explicit MapRef(const Map &map);
  const Map &get() const;
};
bool operator==(const MapRef &lhs, const MapRef &rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
