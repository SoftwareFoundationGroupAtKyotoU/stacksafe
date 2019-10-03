#include "env.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

void Params::insert(const llvm::Value &v) {
  Super::insert(&v);
}

Env::Env(const Cache &c, const Memory &m) : cache_{c}, mem_{m} {}
Env::Env(Cache &c, const llvm::Function &f) : cache_{c} {
  auto g = Domain::global();
  insert(Symbol::global(), g);
  for (const auto &a : f.args()) {
    c.add(a);
    insert(a, g);
  }
  for (const auto &b : f) {
    for (const auto &i : b) {
      if (check_register(i)) {
        c.add(i);
        insert(i, Domain{});
      }
    }
  }
}
Memory Env::memory() const {
  return mem_;
}
void Env::binop(const llvm::Value &dst, const llvm::Value &lhs,
                const llvm::Value &rhs) {
  Domain dom;
  dom.merge(lookup(lhs));
  dom.merge(lookup(rhs));
  insert(dst, dom);
}
void Env::alloc(const llvm::Value &dst) {
  auto sym = Symbol::make();
  insert(sym, Domain{});
  insert(dst, Domain{sym});
}
void Env::load(const llvm::Value &dst, const llvm::Value &src) {
  Domain dom;
  for (const auto &sym : lookup(src)) {
    dom.merge(lookup(sym));
  }
  insert(dst, dom);
}
void Env::store(const llvm::Value &src, const llvm::Value &dst) {
  auto source = lookup(src);
  for (const auto &target : lookup(dst)) {
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
  for (const auto &val : params) {
    dom.merge(lookup(*val));
  }
  insert(dst, dom);
}
void Env::call(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (const auto &val : params) {
    for (const auto &sym : lookup(*val)) {
      collect(sym, dom);
    }
  }
  for (const auto &sym : dom) {
    insert(sym, dom);
    insert(sym, Domain::global());
  }
  if (!check_voidfunc(dst)) {
    insert(dst, dom);
    insert(dst, Domain::global());
  }
}
void Env::constant(const llvm::Value &dst) {
  insert(dst, Domain{});
}
bool Env::has_global(const llvm::Value &arg) const {
  return lookup(arg).includes(Domain::global());
}
bool Env::has_local(const llvm::Value &ret) const {
  return lookup(ret).has_local();
}
bool Env::has_local(const Symbol &sym) const {
  return lookup(sym).has_local();
}
Domain Env::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return mem_.stack().lookup(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain::global();
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
}
Domain Env::lookup(const Symbol &key) const {
  return mem_.heap().lookup(key);
}
void Env::insert(const llvm::Value &key, const Domain &val) {
  mem_.stack().insert(cache_.lookup(key), val);
}
void Env::insert(const Symbol &key, const Domain &val) {
  mem_.heap().insert(key, val);
}
void Env::collect(const Symbol &symbol, Domain &done) const {
  if (done.merge(Domain{symbol})) {
    for (const auto &sym : lookup(symbol)) {
      collect(sym, done);
    }
  }
}

}  // namespace stacksafe
