#include "env.hpp"
#include <llvm/IR/Function.h>
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  Domain dom;
  dom.insert(g);
  insert(g, dom);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    Domain dom;
    dom.insert(arg);
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
  return stack_.lookup(key);
}
void Env::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key.value(), val);
}
void Env::insert(const Register &key, const Domain &val) {
  stack_.insert(key.value(), val);
}

}  // namespace stacksafe
