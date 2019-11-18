#include "symbol.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol(const Value &v) : Value{v} {}
const Value &Symbol::value() const {
  return *this;
}
bool Symbol::is_global() const {
  return !value() || is_constant(value());
}
bool Symbol::is_local() const {
  return is_register(value());
}
bool Symbol::is_arg() const {
  return is_argument(value());
}
const Symbol &Symbol::get_global() {
  static Symbol global{Value{}};
  return global;
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
