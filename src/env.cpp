#include "env.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(Memory& m, RegisterCache& r) : mem_{m}, cache_{r} {}
void Env::insert_stack(const llvm::Value& key, const Domain& val) {
  mem_.insert_stack(Value::make(key), val);
}
void Env::insert_heap(const Symbol& key, const Domain& val) {
  mem_.insert_heap(key, val);
}
Domain Env::from_stack(const llvm::Value& key) {
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
Domain Env::from_heap(const Symbol& key) {
  if (auto dom = mem_.heap().get(key)) {
    return *dom;
  }
  return Domain{};
}

}  // namespace stacksafe
