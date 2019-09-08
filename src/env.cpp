#include "env.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "fabric.hpp"
#include "json.hpp"
#include "token.hpp"

namespace stacksafe {

Env::Env(llvm::Function& f) {
  auto g = Symbol::global();
  heap_.insert(g, g);
  for (auto& a : f.args()) {
    stack_.insert(Value::create(a), g);
  }
}
const Heap& Env::heap() const { return heap_; }
const Stack& Env::stack() const { return stack_; }
bool Env::merge(const Env& that) {
  bool ret = false;
  if (!heap_.includes(that.heap_)) {
    heap_.insert(that.heap_);
    ret = true;
  }
  if (!stack_.includes(that.stack_)) {
    stack_.insert(that.stack_);
    ret = true;
  }
  return ret;
}
bool Env::binop(const Value& dst, const Value& lhs, const Value& rhs) {
  stack_.insert(dst, from_value(lhs));
  stack_.insert(dst, from_value(rhs));
  return true;
}
bool Env::alloc(const Value& dst) {
  if (dst.is_register()) {
    auto sym = Symbol::create(dst.type().pointee_type());
    heap_.insert(sym);
    stack_.insert(dst, sym);
    return true;
  }
  return false;
}
bool Env::load(const Value& dst, const Value& src) {
  if (auto ptr = from_register(src); ptr && dst.is_register()) {
    for (auto& sym : *ptr) {
      if (auto source = heap_.get(sym)) {
        stack_.insert(dst, *source);
        continue;
      }
      return false;
    }
    return true;
  }
  return false;
}
bool Env::store(const Value& src, const Value& dst) {
  if (auto target = from_register(dst)) {
    auto source = from_value(src);
    for (auto& t : *target) {
      heap_.insert(t, source);
    }
    return true;
  }
  return false;
}
bool Env::cmpxchg(const Value& dst, const Value& ptr, const Value& val) {
  return load(dst, ptr) && store(val, ptr);
}
bool Env::cast(const Value& dst, const Value& src) {
  stack_.insert(dst, from_value(src));
  return true;
}
bool Env::phi(const Value& dst, const Params& params) {
  for (auto& val : params) {
    stack_.insert(dst, from_value(val));
  }
  return true;
}
bool Env::call(const Value& dst, const Params& params) {
  auto domain = collect(params);
  for (auto& sym : domain) {
    heap_.insert(sym, domain);
  }
  if (dst.is_register()) {
    stack_.insert(dst, domain);
  }
  return true;
}
bool Env::constant(const Value& dst) {
  stack_.insert(dst);
  return true;
}
Domain* Env::from_register(const Value& reg) {
  return reg.is_register() ? stack_.get(reg) : nullptr;
}
Domain Env::from_value(const Value& reg) const {
  if (auto d = stack_.get(reg)) {
    return *d;
  }
  return Domain{};
}
Domain Env::from_symbol(const Symbol& sym) const {
  if (auto d = heap_.get(sym)) {
    return *d;
  }
  return Domain{};
}
void Env::collect(const Symbol& symbol, Domain& done) const {
  if (!done.includes(symbol)) {
    done.insert(symbol);
    if (auto d = heap_.get(symbol)) {
      for (auto& sym : *d) {
        collect(sym, done);
      }
    }
  }
}
Domain Env::collect(const Params& params) const {
  Domain ret;
  for (auto& val : params) {
    for (auto& sym : from_value(val)) {
      collect(sym, ret);
    }
  }
  return ret;
}
void to_json(Json& j, const Env& x) {
  j["stack"] = x.stack();
  j["heap"] = x.heap();
}
Fabric dump(const Env& env) {
  Fabric ret, tmp;
  tmp.append("heap").quote().append(": ");
  tmp.append(dump(env.heap())).append(",").next();
  tmp.append("stack").quote().append(": ");
  tmp.append(dump(env.stack()));
  ret.append("{").next();
  ret.append(tmp.indent(2)).next();
  return ret.append("}");
}

}  // namespace stacksafe
