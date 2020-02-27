#include "node.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/ErrorHandling.h>
#include "utility.hpp"

namespace stacksafe {

Node::Node(GlobalSymbol g) : Value{g} {}
Node::Node(const llvm::AllocaInst &l) : Value{&l} {}
Node::Node(const Symbol &sym) : Value{sym} {}
Node::Node(const Register &reg) : Value{reg} {}
Node Node::get_symbol(const llvm::AllocaInst &a) {
  return Node{Symbol{a}};
}
Node Node::get_global() {
  return Node{Symbol::get_global()};
}
Node Node::get_register(const llvm::Argument &a) {
  return Node{Register{a}};
}
Node Node::get_register(const llvm::Instruction &i) {
  return Node{Register{i}};
}
Node Node::from_value(const llvm::Value &v) {
  if (auto c = llvm::dyn_cast<llvm::Constant>(&v)) {
    return is_global(*c) ? Node::get_global() : Node{};
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    assert(is_register(*i) && "invalid register lookup");
    return Node::get_register(*i);
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(&v)) {
    return Node::get_register(*a);
  } else {
    llvm_unreachable("invalid value lookup");
  };
}
const void *Node::ptr() const {
  if (auto sym = std::get_if<Symbol>(this)) {
    return sym->value();
  } else if (auto reg = as_register()) {
    return &reg->value();
  } else if (auto g = std::get_if<GlobalSymbol>(this)) {
    return g;
  } else if (auto l = std::get_if<LocalSymbol>(this)) {
    return l;
  } else {
    return nullptr;
  }
}
std::pair<std::size_t, const void *> Node::pair() const {
  return {index(), ptr()};
}
const Register *Node::as_register() const {
  return std::get_if<Register>(this);
}
bool Node::is_symbol() const {
  return std::get_if<Symbol>(this);
}
bool Node::is_local() const {
  return is_symbol() && std::get_if<Symbol>(this)->is_local();
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.pair() == rhs.pair();
}
bool operator<(const Node &lhs, const Node &rhs) {
  return lhs.pair() < rhs.pair();
}

void NodeSet::merge(const NodeSet &nodes) {
  Super::insert(nodes.begin(), nodes.end());
}
bool NodeSet::element(const Node &n) const {
  return 0 != Super::count(n);
}
bool NodeSet::has_local() const {
  for (const auto &n : *this) {
    if (n.is_local()) {
      return true;
    }
  }
  return false;
}

}  // namespace stacksafe
