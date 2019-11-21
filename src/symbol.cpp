#include "symbol.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol() : Value{}, key_{} {}
Symbol::Symbol(const llvm::AllocaInst &val) : Value{val}, key_{val} {}
Symbol::Symbol(const llvm::Argument &val) : Value{val}, key_{val} {}
Symbol::Symbol(const llvm::Instruction &val) : Value{val}, key_{val} {}
const Value &Symbol::value() const {
  return *this;
}
bool Symbol::is_global() const {
  auto v = value().get();
  return !v || llvm::isa<llvm::Constant>(v);
}
bool Symbol::is_local() const {
  if (auto i = llvm::dyn_cast_or_null<llvm::Instruction>(value().get())) {
    return is_register(*i);
  }
  return false;
}
bool Symbol::is_arg() const {
  auto v = value().get();
  return v && llvm::isa<llvm::Argument>(v);
}
Symbol Symbol::get_global() {
  return Symbol{};
}
Symbol Symbol::get_local(const llvm::AllocaInst &v) {
  return Symbol{v};
}
Symbol Symbol::get_arg(const llvm::Argument &v) {
  return Symbol{v};
}
std::size_t Symbol::hash(const Symbol &sym) {
  return Value::hash(sym.value());
}
bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() == rhs.value();
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() < rhs.value();
}

}  // namespace stacksafe
