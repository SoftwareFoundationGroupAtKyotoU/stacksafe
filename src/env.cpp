#include "env.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "json.hpp"
#include "value.hpp"

namespace stacksafe {

Env::Env(llvm::Function& f) {
  for (auto& a : f.args()) {
    argument(Value{a});
  }
}
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
void Env::argument(const Value& arg) { alloc(arg); }
void Env::alloc(const Value& dst) {
  auto type = dst.type();
  auto sym = Symbol::create(type.pointee_type());
  heap_.insert(sym);
  stack_.insert(dst, sym);
}
void Env::store(const Value& src, const Value& dst) {
  auto source = stack_.get(src);
  auto target = stack_.get(dst);
  if (source && target) {
    for (auto& t : *target) {
      heap_.insert(t, *source);
    }
  }
}
void Env::load(const Value& dst, const Value& src) {
  if (auto ptr = stack_.get(src)) {
    for (auto& sym : *ptr) {
      if (auto source = heap_.get(sym)) {
        stack_.insert(dst, *source);
      }
    }
  }
}
void Env::constant(const Value& dst) { stack_.insert(dst); }
void Env::call(const Value& dst, const Params& params) {
  auto domain = collect(params);
  for (auto& sym : domain) {
    heap_.insert(sym, domain);
  }
  if (dst.is_register()) {
    stack_.insert(dst, domain);
  }
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
    if (auto d = stack_.get(val)) {
      for (auto& sym : *d) {
        collect(sym, ret);
      }
    }
  }
  return ret;
}
void to_json(Json& j, const Env& x) {
  j["stack"] = x.stack_;
  j["heap"] = x.heap_;
}

}  // namespace stacksafe
