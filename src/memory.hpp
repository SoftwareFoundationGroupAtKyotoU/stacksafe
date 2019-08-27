#ifndef INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
#define INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944

#include <map>
#include <set>
#include "env.hpp"
#include "json.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Stack : private Env<Register> {
  using Base = Env<Register>;

 public:
  using Base::begin, Base::end, Base::get, Base::insert;
};
void to_json(Json& j, const Stack& x);

class Heap : private Env<Symbol> {
  using Base = Env<Symbol>;

 public:
  using Base::begin, Base::end, Base::get, Base::insert;
};
void to_json(Json& j, const Heap& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
