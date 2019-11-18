#include "env.hpp"
#include <llvm/IR/Function.h>
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const llvm::Function &f) {
  insert(Symbol::get_global(), Domain::get_global());
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    const auto dom = Domain::get_singleton(arg);
    insert(arg, dom);
    insert(Register::make(a), dom);
  }
}
const Map &Env::heap() const {
  return heap_;
}
const Map &Env::stack() const {
  return stack_;
}
bool Env::includes(const Env &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void Env::merge(const Env &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
Domain Env::lookup(const Symbol &key) const {
  return heap_.lookup(key.value());
}
Domain Env::lookup(const Value &key) const {
  using K = Value::Kind;
  switch (key.kind()) {
    case K::REGISTER:
      [[fallthrough]];
    case K::ARGUMENT:
      return stack_.lookup(key);
    case K::GLOBAL:
      return Domain::get_global();
    case K::CONSTANT:
      return Domain::get_empty();
    case K::OTHER:
      [[fallthrough]];
    default:
      stacksafe_unreachable("unregistered register", key);
  }
}
void Env::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key.value(), val);
}
void Env::insert(const Register &key, const Domain &val) {
  stack_.insert(key.value(), val);
}

}  // namespace stacksafe
