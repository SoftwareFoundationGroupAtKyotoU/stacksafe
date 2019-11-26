#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <vector>
#include "value.hpp"

namespace stacksafe {

class Domain : private std::vector<Value> {
  using Super = std::vector<Value>;

 public:
  using Super::begin, Super::end, Super::empty;
  Domain() = default;
  explicit Domain(const Value &val);
  bool insert(const Value &sym);
  bool merge(const Domain &that);
  bool element(const Value &sym) const;
  bool has_local() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
