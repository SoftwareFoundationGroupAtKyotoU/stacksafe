#include "env.hpp"
#include <llvm/IR/Function.h>
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

EnvSlice::EnvSlice(const llvm::Function &f) {
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
const Map &EnvSlice::heap() const {
  return heap_;
}
const Map &EnvSlice::stack() const {
  return stack_;
}
bool EnvSlice::includes(const EnvSlice &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void EnvSlice::merge(const EnvSlice &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
Domain EnvSlice::lookup(const Symbol &key) const {
  return heap_.lookup(key.value());
}
Domain EnvSlice::lookup(const Value &key) const {
  return stack_.lookup(key);
}
void EnvSlice::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key.value(), val);
}
void EnvSlice::insert(const Register &key, const Domain &val) {
  stack_.insert(key.value(), val);
}

}  // namespace stacksafe
