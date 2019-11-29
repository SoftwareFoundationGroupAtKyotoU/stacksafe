#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <vector>
#include "symbol.hpp"

namespace stacksafe {
class Value;

class Domain : private std::vector<Symbol> {
  using Super = std::vector<Symbol>;

 public:
  using Super::begin, Super::end, Super::empty;
  Domain() = default;
  explicit Domain(const Value &val);
  bool insert(const Symbol &val);
  bool merge(const Domain &dom);
  bool element(const Symbol &val) const;
  bool has_local() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
