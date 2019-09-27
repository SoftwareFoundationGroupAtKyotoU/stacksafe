#include "env.hpp"
#include <llvm/IR/Value.h>
#include "instruction.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const Memory& m, RegisterCache& c) : mem_{m}, cache_{c} {}
Memory Env::memory() const { return mem_; }
Memory& Env::memory() { return mem_; }
void Env::insert_stack(const llvm::Value& key, const Domain& val) {
  mem_.insert_stack(Value::make(key), val);
}
void Env::insert_heap(const Symbol& key, const Domain& val) {
  mem_.insert_heap(key, val);
}
Domain Env::from_stack(const llvm::Value& key) const {
  if (check_register(key)) {
    if (auto dom = mem_.stack().get(Value::make(key))) {
      return *dom;
    } else {
      return Domain{};
    }
  } else if (check_global(key)) {
    return Domain{Symbol::global()};
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
}
Domain Env::from_heap(const Symbol& key) const {
  if (auto dom = mem_.heap().get(key)) {
    return *dom;
  }
  return Domain{};
}
Domain Env::collect(const ValueSet& params) const {
  Domain ret;
  for (auto& val : params) {
    assert(val && "invalid param");
    for (auto& sym : from_stack(*val)) {
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
void Env::binop(const llvm::Value& dst, const llvm::Value& lhs,
                const llvm::Value& rhs) {
  Domain dom;
  dom.insert(from_stack(lhs));
  dom.insert(from_stack(rhs));
  insert_stack(dst, dom);
}
void Env::alloc(const llvm::Value& dst) {
  auto sym = Symbol::make(Type{dst.getType()}.pointee_type());
  insert_heap(sym, Domain{});
  insert_stack(dst, Domain{sym});
}
void Env::load(const llvm::Value& dst, const llvm::Value& src) {
  Domain dom;
  for (auto& sym : from_stack(src)) {
    dom.insert(from_heap(sym));
  }
  insert_stack(dst, dom);
}

}  // namespace stacksafe
