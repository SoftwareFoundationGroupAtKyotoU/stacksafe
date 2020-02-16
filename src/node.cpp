#include "node.hpp"
#include <llvm/Support/ErrorHandling.h>

namespace stacksafe {

Node::Node(const Symbol &sym) : Super{sym} {}
Node::Node(const Register &reg) : Super{reg} {}
std::uintptr_t Node::value() const {
  if (auto sym = as_symbol()) {
    return reinterpret_cast<std::uintptr_t>(sym->value());
  } else if (auto reg = as_register()) {
    return reinterpret_cast<std::uintptr_t>(&reg->value());
  } else {
    llvm_unreachable("Node is either symbol or register");
  }
}
const Symbol *Node::as_symbol() const {
  return std::get_if<Symbol>(this);
}
const Register *Node::as_register() const {
  return std::get_if<Register>(this);
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.index() == rhs.index() && lhs.value() == rhs.value();
}
bool operator<(const Node &lhs, const Node &rhs) {
  if (lhs.index() == rhs.index()) {
    return lhs.value() < rhs.value();
  } else {
    return lhs.index() < rhs.index();
  }
}

}  // namespace stacksafe
