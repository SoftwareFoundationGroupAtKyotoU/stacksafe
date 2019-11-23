#ifndef INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
#define INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E

#include <unordered_map>
#include "map.hpp"
#include "value.hpp"

namespace stacksafe {

class Env : private std::unordered_multimap<Value, MapRef> {
  using Super = std::unordered_multimap<Value, MapRef>;

 public:
  Domain lookup(const Value& key) const;
};

}  // namespace stacksafe
#endif  // INCLUDE_GUARD_DA7CDD8A_EA48_45CA_B057_F470D4F72F4E
