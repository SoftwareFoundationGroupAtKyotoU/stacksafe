#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <unordered_set>
#include "symbol.hpp"

namespace stacksafe {

class Domain : private std::unordered_set<Symbol> {
  using Super = std::unordered_set<Symbol>;
  Domain() = default;

 public:
  using Super::begin, Super::end;
  void merge(const Domain &that);
  bool includes(const Domain &that) const;
  Domain minus(const Domain &that) const;
  static Domain get_empty();
  static Domain get_global();
  static Domain get_singleton(const Symbol &sym);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
