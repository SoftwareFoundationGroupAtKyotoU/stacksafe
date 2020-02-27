#include "node.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/ErrorHandling.h>
#include "utility.hpp"

namespace stacksafe {

Node Node::get_global() {
  return Node{GlobalSymbol{}};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{LocalSymbol{&l}};
}
Node Node::get_register(const llvm::Argument &a) {
  return Node{LocalArgument{&a}};
}
Node Node::get_register(const llvm::Instruction &i) {
  return Node{LocalRegister{&i}};
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
  if (auto g = std::get_if<GlobalSymbol>(this)) {
    return nullptr;
  } else if (auto l = std::get_if<LocalSymbol>(this)) {
    return l->get();
  } else if (auto r = std::get_if<LocalRegister>(this)) {
    return r->get();
  } else if (auto a = std::get_if<LocalArgument>(this)) {
    return a->get();
  } else {
    return reinterpret_cast<const void *>(-1);
  }
}
std::pair<std::size_t, const void *> Node::pair() const {
  return {index(), ptr()};
}
bool Node::is_symbol() const {
  return std::get_if<GlobalSymbol>(this) || std::get_if<LocalSymbol>(this);
}
bool Node::is_local() const {
  return std::get_if<LocalSymbol>(this);
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
