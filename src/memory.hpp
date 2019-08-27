#ifndef INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
#define INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944

#include <map>
#include <set>
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Stack : private std::map<Register, std::set<Symbol>> {};
class Heap : private std::map<Symbol, std::set<Symbol>> {};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_131C5103_4229_44F2_8D1D_95A9AC6AC944
