#include "node.hpp"
#include <llvm/Support/ErrorHandling.h>

namespace stacksafe {

Node::Node(const Symbol &sym) : Super{sym} {}
Node::Node(const Register &reg) : Super{reg} {}
std::uintptr_t Node::value() const {
  if (auto sym = std::get_if<Symbol>(this)) {
    return reinterpret_cast<std::uintptr_t>(sym->value());
  } else if (auto reg = std::get_if<Register>(this)) {
    return reinterpret_cast<std::uintptr_t>(&reg->value());
  } else {
    llvm_unreachable("Node is either symbol or register");
  }
}

}  // namespace stacksafe
