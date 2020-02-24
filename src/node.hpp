#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <cstdint>
#include <tuple>
#include <variant>
#include "register.hpp"
#include "symbol.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {

class Node : private std::variant<std::monostate, Symbol, Register> {
  using Super = std::variant<std::monostate, Symbol, Register>;

 public:
  static Node get_symbol(const llvm::AllocaInst &a);
  static Node get_symbol(const llvm::Argument &a);
  static Node get_global();
  static Node get_register(const llvm::Argument &a);
  static Node get_register(const llvm::Instruction &i);
  static Node from_value(const llvm::Value &v);
  Node() = default;
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
  explicit Edge(const Node &n);
  const Node &tail() const;
  const Node &head() const;
  const Super &pair() const;
};
bool operator==(const Edge &lhs, const Edge &rhs);
bool operator<(const Edge &lhs, const Edge &rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
