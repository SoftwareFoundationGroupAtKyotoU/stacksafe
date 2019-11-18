#include "symbol.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol(const Value &v) : Value{v} {}
const Value &Symbol::value() const {
  return *this;
}
bool Symbol::is_global() const {
  auto v = value().get();
  return !v || llvm::isa<llvm::Constant>(v);
}
bool Symbol::is_local() const {
  return is_register(value());
}
bool Symbol::is_arg() const {
  auto v = value().get();
  return v && llvm::isa<llvm::Argument>(v);
}
Symbol Symbol::get_global() {
  return Symbol{Value{}};
}
Symbol Symbol::get_local(const llvm::AllocaInst &v) {
  return Symbol{v};
}
Symbol Symbol::get_arg(const llvm::Argument &v) {
  return Symbol{v};
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() < rhs.value();
}

}  // namespace stacksafe
