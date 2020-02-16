#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <variant>
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Node : std::variant<Symbol, Register> {
  using Super = std::variant<Symbol, Register>;

 public:
  explicit Node(const Symbol &sym);
  explicit Node(const Register &reg);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
