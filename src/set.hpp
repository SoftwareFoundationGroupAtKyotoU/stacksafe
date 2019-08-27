#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <set>
#include "json.hpp"
#include "symbol.hpp"

namespace stacksafe {

template <typename T>
class Set : private std::set<T> {
  using Base = std::set<T>;
};

class Domain : private std::set<Symbol> {
  using Base = std::set<Symbol>;

 public:
  using Base::Base, Base::begin, Base::end;
};
void to_json(Json& j, const Domain& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
