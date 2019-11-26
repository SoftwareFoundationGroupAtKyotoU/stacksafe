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

 public:
  Map() = default;
  explicit Map(const llvm::Function &f);
  using Super::begin, Super::end;
  bool insert(const Value &key, const Value &val);
  bool insert(const Value &key, const Domain &dom);
  Domain lookup(const Value &key) const;
  static Domain keys(const Map &map);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
