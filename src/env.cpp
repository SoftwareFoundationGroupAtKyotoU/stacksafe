#include "env.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(const Memory& m) : mem_{m} {}
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

}  // namespace stacksafe
