#ifndef INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
#define INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E

#include <unordered_map>
#include "map.hpp"
#include "value.hpp"

namespace stacksafe {

class Env : private std::unordered_multimap<Value, MapRef> {
  using Super = std::unordered_multimap<Value, MapRef>;
  using Super::const_iterator;

 public:
  Domain lookup(const Value& key) const;
  static bool range_contains(const_iterator lb, const_iterator ub,
                             const Value& val);
};

}  // namespace stacksafe
#endif  // INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
