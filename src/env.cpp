#include "env.hpp"
#include <llvm/IR/Function.h>
#include "domain.hpp"
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
void Env::binop(const Value& dst, const Value& lhs, const Value& rhs) {
  Domain dom;
  dom.insert(from_stack(lhs));
  dom.insert(from_stack(rhs));
  insert_stack(dst, dom);
}
void Env::alloc(const Value& dst) {
  auto sym = Symbol::make(dst.type().pointee_type());
  insert_heap(sym, Domain{});
  insert_stack(dst, Domain{sym});
}
void Env::load(const Value& dst, const Value& src) {
  Domain dom;
  for (auto& sym : from_stack(src)) {
    dom.insert(from_heap(sym));
  }
  insert_stack(dst, dom);
}
void Env::store(const Value& src, const Value& dst) {
  auto source = from_stack(src);
  for (auto& target : from_stack(dst)) {
    insert_heap(target, source);
  }
}
void Env::cmpxchg(const Value& dst, const Value& ptr, const Value& val) {
  load(dst, ptr);
  store(val, ptr);
}
void Env::cast(const Value& dst, const Value& src) {
  insert_stack(dst, from_stack(src));
}
void Env::phi(const Value& dst, const Params& params) {
  Domain dom;
  for (auto& val : params) {
    dom.insert(from_stack(val));
  }
  insert_stack(dst, dom);
}
Domain Env::call(const Params& params) {
  auto domain = collect(params);
  for (auto& sym : domain) {
    insert_heap(sym, domain);
  }
  return domain;
}
void Env::call(const Value& dst, const Params& params) {
  insert_stack(dst, call(params));
}
void Env::call(const Domain& dom) {
  for (auto& sym : dom) {
    insert_heap(sym, dom);
  }
}
void Env::constant(const Value& dst) { insert_stack(dst, Domain{}); }
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
void Env::collect(const Symbol& symbol, Domain& done) const {
  if (!done.includes(symbol)) {
    done.insert(symbol);
    for (auto& sym : from_heap(symbol)) {
      collect(sym, done);
    }
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
void to_json(Json& j, const Env& x) {
  j["stack"] = x.stack();
  j["heap"] = x.heap();
}

void binop(Env& e, const Value& dst, const Value& lhs, const Value& rhs) {
  Domain dom;
  dom.insert(e.from_stack(lhs));
  dom.insert(e.from_stack(rhs));
  e.insert_stack(dst, dom);
}
void alloc(Env& e, const Value& dst) {
  auto sym = Symbol::make(dst.type().pointee_type());
  e.insert_heap(sym, Domain{});
  e.insert_stack(dst, Domain{sym});
}
void load(Env& e, const Value& dst, const Value& src) {
  Domain dom;
  for (auto& sym : e.from_stack(src)) {
    dom.insert(e.from_heap(sym));
  }
  e.insert_stack(dst, dom);
}
void store(Env& e, const Value& src, const Value& dst) {
  auto source = e.from_stack(src);
  for (auto& target : e.from_stack(dst)) {
    e.insert_heap(target, source);
  }
}
void cmpxchg(Env& e, const Value& dst, const Value& ptr, const Value& val) {
  e.load(dst, ptr);
  e.store(val, ptr);
}
void cast(Env& e, const Value& dst, const Value& src) {
  e.insert_stack(dst, e.from_stack(src));
}
void phi(Env& e, const Value& dst, const Params& params) {
  Domain dom;
  for (auto& val : params) {
    dom.insert(e.from_stack(val));
  }
  e.insert_stack(dst, dom);
}

}  // namespace stacksafe
