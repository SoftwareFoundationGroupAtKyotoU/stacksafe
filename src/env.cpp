#include "env.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include "cache.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const Memory &m, RegisterCache &c) : mem_{m}, cache_{c} {}
Env::Env(const llvm::Function &f, RegisterCache &c) : cache_{c} {
  auto g = Symbol::global();
  Domain dom{g};
  insert(g, dom);
  for (auto &a : f.args()) {
    insert(a, dom);
  }
}
Memory Env::memory() const {
  return mem_;
}
Domain Env::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return mem_.stack().lookup(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain{Symbol::global()};
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
}
Domain Env::lookup(const Symbol &key) const {
  return mem_.heap().lookup(key);
}
void Env::binop(const llvm::Value &dst, const llvm::Value &lhs,
                const llvm::Value &rhs) {
  Domain dom;
  dom.insert(lookup(lhs));
  dom.insert(lookup(rhs));
  insert(dst, dom);
}
void Env::alloc(const llvm::Value &dst) {
  auto sym = Symbol::make(Type{dst.getType()}.pointee_type());
  insert(sym, Domain{});
  insert(dst, Domain{sym});
}
void Env::load(const llvm::Value &dst, const llvm::Value &src) {
  Domain dom;
  for (auto &sym : lookup(src)) {
    dom.insert(lookup(sym));
  }
  insert(dst, dom);
}
void Env::store(const llvm::Value &src, const llvm::Value &dst) {
  auto source = lookup(src);
  for (auto &target : lookup(dst)) {
    insert(target, source);
  }
}
void Env::cmpxchg(const llvm::Value &dst, const llvm::Value &ptr,
                  const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Env::cast(const llvm::Value &dst, const llvm::Value &src) {
  insert(dst, lookup(src));
}
void Env::phi(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (auto &val : params) {
    assert(val && "invalid param");
    dom.insert(lookup(*val));
  }
  insert(dst, dom);
}
void Env::call(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (auto &val : params) {
    assert(val && "invalid param");
    for (auto &sym : lookup(*val)) {
      collect(sym, dom);
    }
  }
  for (auto &sym : dom) {
    insert(sym, dom);
  }
  if (!check_voidfunc(dst)) {
    insert(dst, dom);
  }
}
void Env::constant(const llvm::Value &dst) {
  insert(dst, Domain{});
}

void Env::insert(const llvm::Value &key, const Domain &val) {
  mem_.stack().insert(cache_.lookup(key), val);
}
void Env::insert(const Symbol &key, const Domain &val) {
  mem_.heap().insert(key, val);
}
void Env::collect(const Symbol &symbol, Domain &done) const {
  Domain single{symbol};
  if (!done.includes(single)) {
    done.insert(single);
    for (auto &sym : lookup(symbol)) {
      collect(sym, done);
    }
  }
}

}  // namespace stacksafe
