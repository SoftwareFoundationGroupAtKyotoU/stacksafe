#ifndef INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
#define INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944

#include <map>
#include <set>
#include "json.hpp"
#include "map.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Stack : private Map<Register> {
  using Base = Map<Register>;

 public:
  using Base::begin, Base::end, Base::get, Base::insert;
};
void to_json(Json& j, const Stack& x);

class Heap : private Map<Symbol> {
  using Base = Map<Symbol>;

 public:
  using Base::begin, Base::end, Base::get, Base::insert;
};
void to_json(Json& j, const Heap& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
