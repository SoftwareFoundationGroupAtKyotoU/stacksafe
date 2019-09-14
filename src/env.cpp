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
    if (stack().exists(val)) {
      dom.insert(from_stack(val));
    }
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
void Env::constant(const Value& dst) { insert_stack(dst, Domain{}); }
void Env::insert_stack(const Value& key, const Domain& val) {
  if (key.is_register()) {
    stack_.insert(key, val);
  } else {
    stacksafe_unreachable("Error: insert to non-register: " + to_str(key));
  }
}
void Env::insert_heap(const Symbol& key, const Domain& val) {
  heap_.insert(key, val);
}
Domain Env::from_stack(const Value& reg) const {
  if (reg.is_register()) {
    if (auto d = stack_.get(reg)) {
      return *d;
    }
    stacksafe_unreachable("Error: read from nonexistent register: " +
                          to_str(reg));
  } else if (check_constant(*reg.get())) {
    print_stdout(to_str(reg));
    return Domain{};
  } else {
    stacksafe_unreachable("Error: read from neither register nor constant: " +
                          to_str(reg));
  }
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

}  // namespace stacksafe
