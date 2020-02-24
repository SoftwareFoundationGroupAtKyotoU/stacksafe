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
const llvm::Value *Node::ptr() const {
  if (auto sym = as_symbol()) {
    return sym->value();
  } else if (auto reg = as_register()) {
    return &reg->value();
  } else {
    return nullptr;
  }
}
std::pair<std::size_t, std::uintptr_t> Node::pair() const {
  return {index(), value()};
}
const Symbol *Node::as_symbol() const {
  return std::get_if<Symbol>(this);
}
const Register *Node::as_register() const {
  return std::get_if<Register>(this);
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.pair() == rhs.pair();
}
bool operator<(const Node &lhs, const Node &rhs) {
  return lhs.pair() < rhs.pair();
}

const Node &Edge::key() const {
  return std::get<0>(*this);
}
const Node &Edge::val() const {
  return std::get<1>(*this);
}
auto Edge::pair() const -> const Super & {
  return *this;
}
bool operator==(const Edge &lhs, const Edge &rhs) {
  return lhs.pair() == rhs.pair();
}
bool operator<(const Edge &lhs, const Edge &rhs) {
  return lhs.pair() < rhs.pair();
}

}  // namespace stacksafe
