#include "env.hpp"
#include <llvm/IR/Function.h>
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

FlatEnv::FlatEnv(const llvm::Function &f) {
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
FlatEnv::FlatEnv(const Map &heap, const Map &stack)
    : heap_{heap}, stack_{stack} {}
const Map &FlatEnv::heap() const {
  return heap_;
}
const Map &FlatEnv::stack() const {
  return stack_;
}
bool FlatEnv::includes(const FlatEnv &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void FlatEnv::merge(const FlatEnv &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
Domain FlatEnv::lookup(const Symbol &key) const {
  return heap_.lookup(key.value());
}
Domain FlatEnv::lookup(const Value &key) const {
  return stack_.lookup(key);
}
void FlatEnv::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key.value(), val);
}
void FlatEnv::insert(const Register &key, const Domain &val) {
  stack_.insert(key.value(), val);
}

void Env::merge(const FlatEnv &env) {
  heap_.insert(env.heap());
  stack_.insert(env.stack());
}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
FlatEnv Env::concat() const {
  Map heap, stack;
  for (const auto &m : heap_) {
    heap.merge(m);
  }
  for (const auto &m : stack_) {
    stack.merge(m);
  }
  return FlatEnv{heap, stack};
}

}  // namespace stacksafe
