#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <vector>
#include "symbol.hpp"

namespace stacksafe {

class Domain : private std::vector<Symbol> {
  using Super = std::vector<Symbol>;

 public:
  using Super::begin, Super::end, Super::empty;
  Domain() = default;
  void insert(const Symbol &sym);
  void merge(const Domain &that);
  bool element(const Symbol &sym) const;
  bool includes(const Domain &that) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
