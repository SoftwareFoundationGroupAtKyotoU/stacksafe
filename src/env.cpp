#include "env.hpp"
#include <llvm/IR/Function.h>
#include "json.hpp"
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const llvm::Function &f) {
  insert(Symbol::get_global(), Domain::get_global());
  for (const auto &a : f.args()) {
    insert(a, Domain::get_global());
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
const Domain &Env::lookup(const Symbol &key) const {
  return heap_.lookup(key.value());
}
const Domain &Env::lookup(const Register &key) const {
  using K = Register::Kind;
  switch (key.kind()) {
    case K::REGISTER:
      return stack_.lookup(key.value());
    case K::GLOBAL:
      return Domain::get_global();
    case K::CONSTANT:
      return Domain::get_empty();
    case K::OTHER:
      stacksafe_unreachable("unregistered register", *key.value());
  }
}
void Env::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key.value(), val);
}
void Env::insert(const Register &key, const Domain &val) {
  stack_.insert(key.value(), val);
}
void Env::collect(const Symbol &curr, Domain &done) const {
  if (!done.includes(Domain::get_singleton(curr))) {
    done.merge(Domain::get_singleton(curr));
    for (const auto &next : lookup(curr)) {
      collect(next, done);
    }
  }
}
void to_json(Json &j, const Env &x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
