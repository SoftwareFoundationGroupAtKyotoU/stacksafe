#include "env.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include "cache.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const Memory &m, RegisterCache &c) : mem_{m}, cache_{c} {}
Env::Env(const llvm::Function &f, RegisterCache &c) : cache_{c} {
  auto g = Symbol::global();
  Domain dom{g};
  insert_heap(g, dom);
  for (auto &a : f.args()) {
    insert_stack(a, dom);
  }
}
Memory Env::memory() const {
  return mem_;
}
Memory &Env::memory() {
  return mem_;
}
void Env::insert_stack(const llvm::Value &key, const Domain &val) {
  mem_.insert_stack(cache_.lookup(key), val);
}
void Env::insert_heap(const Symbol &key, const Domain &val) {
  mem_.insert_heap(key, val);
}
Domain Env::from_stack(const llvm::Value &key) const {
  if (check_register(key)) {
    return mem_.from_stack(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain{Symbol::global()};
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
}
Domain Env::from_heap(const Symbol &key) const {
  return mem_.from_heap(key);
}
Domain Env::collect(const Params &params) const {
  Domain ret;
  for (auto &val : params) {
    assert(val && "invalid param");
    for (auto &sym : from_stack(*val)) {
      collect(sym, ret);
    }
  }
  return ret;
}
void Env::collect(const Symbol &symbol, Domain &done) const {
  if (!done.includes(symbol)) {
    done.insert(symbol);
    for (auto &sym : from_heap(symbol)) {
      collect(sym, done);
    }
  }
}
void Env::binop(const llvm::Value &dst, const llvm::Value &lhs,
                const llvm::Value &rhs) {
  Domain dom;
  dom.insert(from_stack(lhs));
  dom.insert(from_stack(rhs));
  insert_stack(dst, dom);
}
void Env::alloc(const llvm::Value &dst) {
  auto sym = Symbol::make(Type{dst.getType()}.pointee_type());
  insert_heap(sym, Domain{});
  insert_stack(dst, Domain{sym});
}
void Env::load(const llvm::Value &dst, const llvm::Value &src) {
  Domain dom;
  for (auto &sym : from_stack(src)) {
    dom.insert(from_heap(sym));
  }
  insert_stack(dst, dom);
}
void Env::store(const llvm::Value &src, const llvm::Value &dst) {
  auto source = from_stack(src);
  for (auto &target : from_stack(dst)) {
    insert_heap(target, source);
  }
}
void Env::cmpxchg(const llvm::Value &dst, const llvm::Value &ptr,
                  const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Env::cast(const llvm::Value &dst, const llvm::Value &src) {
  insert_stack(dst, from_stack(src));
}
void Env::phi(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (auto &val : params) {
    assert(val && "invalid param");
    dom.insert(from_stack(*val));
  }
  insert_stack(dst, dom);
}
void Env::call(const llvm::Value &dst, const Params &params) {
  auto dom = collect(params);
  for (auto &sym : dom) {
    insert_heap(sym, dom);
  }
  if (!check_voidfunc(dst)) {
    insert_stack(dst, dom);
  }
}
void Env::constant(const llvm::Value &dst) {
  insert_stack(dst, Domain{});
}

}  // namespace stacksafe
