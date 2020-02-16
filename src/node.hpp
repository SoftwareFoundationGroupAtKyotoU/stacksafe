#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <variant>
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Node : std::variant<Symbol, Register> {
  using Super = std::variant<Symbol, Register>;

 public:
  using Super::index;
  explicit Node(const Symbol &sym);
  explicit Node(const Register &reg);
  std::uintptr_t value() const;
  const llvm::Value *ptr() const;
  const Symbol *as_symbol() const;
  const Register *as_register() const;
};
bool operator==(const Node &lhs, const Node &rhs);
bool operator<(const Node &lhs, const Node &rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
