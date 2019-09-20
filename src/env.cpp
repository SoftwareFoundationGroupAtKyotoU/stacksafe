#include "env.hpp"
#include <llvm/IR/Function.h>
#include "domain.hpp"
#include "instruction.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Env::Env(const llvm::Function& f) {
  auto g = Symbol::global();
  insert_heap(g, Domain{g});
  for (auto& a : f.args()) {
    insert_stack(Value::make(a), Domain{g});
  }
}
const Heap& Env::heap() const { return heap_; }
const Stack& Env::stack() const { return stack_; }
bool Env::includes(const Env& that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void Env::merge(const Env& that) {
  heap_.insert(that.heap_);
  stack_.insert(that.stack_);
}
void Env::insert_stack(const Value& key, const Domain& val) {
  assert(key.is_register() && "insert to non-register");
  stack_.insert(key, val);
}
void Env::insert_heap(const Symbol& key, const Domain& val) {
  heap_.insert(key, val);
}
Domain Env::from_stack(const Value& reg) const {
  if (reg.is_register()) {
    // comparison on only registers is allowed
    if (auto d = stack_.get(reg)) {
      return *d;
    }
  }
  return reg.get_domain();
}
Domain Env::from_heap(const Symbol& sym) const {
  if (auto d = heap_.get(sym)) {
    return *d;
  }
  return Domain{};
}
void Env::call(const Domain& dom) {
  for (auto& sym : dom) {
    insert_heap(sym, dom);
  }
}
Domain Env::collect(const Params& params) const {
  Domain ret;
  for (auto& val : params) {
    for (auto& sym : from_stack(val)) {
      collect(sym, ret);
    }
  }
  return ret;
}
void Env::collect(const Symbol& symbol, Domain& done) const {
  if (!done.includes(symbol)) {
    done.insert(symbol);
    for (auto& sym : from_heap(symbol)) {
      collect(sym, done);
    }
  }
}
void to_json(Json& j, const Env& x) {
  j["stack"] = x.stack();
  j["heap"] = x.heap();
}

}  // namespace stacksafe
