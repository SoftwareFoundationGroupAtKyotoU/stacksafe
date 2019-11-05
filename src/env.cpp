#include "env.hpp"
#include <llvm/IR/Function.h>
#include "json.hpp"
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
  return heap_.lookup(key);
}
const Domain &Env::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return stack_.lookup(Symbol::get_local(key));
  } else if (check_global(key)) {
    return Domain::get_global();
  } else if (check_constant(key)) {
    return Domain::get_empty();
  }
  stacksafe_unreachable("neither register nor constant", key);
}
void Env::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key, val);
}
void Env::insert(const llvm::Value &key, const Domain &val) {
  stack_.insert(Symbol::get_local(key), val);
}
void Env::collect(const Symbol &curr, Domain &done) const {
  if (!done.includes(Domain{curr})) {
    done.merge(Domain{curr});
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
