#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <cstdint>
#include <tuple>
#include <variant>
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Node : private std::variant<Symbol, Register> {
  using Super = std::variant<Symbol, Register>;

 public:
  explicit Node(const Symbol &sym);
  explicit Node(const Register &reg);
  std::uintptr_t value() const;
  const llvm::Value *ptr() const;
  std::pair<std::size_t, std::uintptr_t> pair() const;
  const Symbol *as_symbol() const;
  const Register *as_register() const;
};
bool operator==(const Node &lhs, const Node &rhs);
bool operator<(const Node &lhs, const Node &rhs);

class Edge : private std::tuple<Node, Node> {
  using Super = std::tuple<Node, Node>;

 public:
  using Super::Super;
  bool operator==(const Edge &that) const;
  bool operator<(const Edge &that) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
