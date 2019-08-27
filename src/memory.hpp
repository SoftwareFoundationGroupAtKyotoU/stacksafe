#ifndef INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
#define INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944

#include <map>
#include <set>
#include "json.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Stack : private std::map<Register, std::set<Symbol>> {
  using Key = Register;
  using Value = std::set<Symbol>;
  using Base = std::map<Key, Value>;

 public:
  using Base::begin, Base::end;
};
void to_json(Json& j, const Stack& x);

class Heap : private std::map<Symbol, std::set<Symbol>> {
  using Key = Symbol;
  using Value = std::set<Symbol>;
  using Base = std::map<Key, Value>;

 public:
  using Base::begin, Base::end;
};
void to_json(Json& j, const Heap& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
