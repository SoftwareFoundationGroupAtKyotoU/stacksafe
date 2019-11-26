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
  friend class MapPool;
  using Super = std::unordered_multimap<Value, Value>;
  std::size_t hash_;
  explicit Map(std::size_t count);
  Map(std::size_t count, const llvm::Function &f);

 public:
  Map();
  using Super::begin, Super::end, Super::empty;
  using Super::value_type;
  bool insert(const Value &key, const Value &val);
  bool insert(const Value &key, const Domain &dom);
  Domain lookup(const Value &key) const;
  bool contains(const Value &key, const Value &val) const;
  bool equals(const Map &map) const;
  static Domain keys(const Map &map);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
