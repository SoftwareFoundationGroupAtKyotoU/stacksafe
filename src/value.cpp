#include "value.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "hash.hpp"

namespace stacksafe {

Value::Value(const Base &base) : Base{base} {}
Value::Value(const llvm::Value &val) : Base{val} {}
Value::Value(const void *sym, bool is_local) : Base{sym, is_local} {}
const llvm::Value *Value::value() const {
  return Base::value();
}
bool Value::is_global() const {
  return Base::is_global();
}
bool Value::is_local() const {
  return Base::is_local();
}
bool Value::is_argument() const {
  return !is_local() && !is_global();
}
Value Value::get_zero() {
  return Value{Base::get_zero()};
}
Value Value::get_negative() {
  return Value{Base::get_negative()};
}
Value Value::get_symbol() {
  return Value{nullptr, false};
}
Value Value::get_symbol(const llvm::AllocaInst &v) {
  return Value{&v, true};
}
Value Value::get_symbol(const llvm::Argument &v) {
  return Value{&v, false};
}
Value Value::get_register(const llvm::Argument &v) {
  return Value{v};
}
Value Value::get_register(const llvm::Instruction &v) {
  return Value{v};
}
bool Value::operator==(const Value &sym) const {
  return Base::equals(*this, sym);
}
bool Value::operator<(const Value &sym) const {
  return Base::less(*this, sym);
}

Pair::Pair(const Value &key, const Value &val) : Super{key, val} {}
const Value &Pair::key() const {
  return std::get<0>(*this);
}
const Value &Pair::val() const {
  return std::get<1>(*this);
}
Pair Pair::get_zero() {
  const auto zero = Value::get_zero();
  return Pair{zero, zero};
}
Pair Pair::get_negative() {
  const auto negative = Value::get_negative();
  return Pair{negative, negative};
}
bool Pair::operator==(const Pair &pair) const {
  const Super &lhs = *this;
  const Super &rhs = pair;
  return lhs == rhs;
}
bool Pair::operator<(const Pair &pair) const {
  const Super &lhs = *this;
  const Super &rhs = pair;
  return lhs < rhs;
}

}  // namespace stacksafe
