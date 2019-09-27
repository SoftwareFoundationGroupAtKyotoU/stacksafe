#include "env.hpp"
#include "memory.hpp"

namespace stacksafe {

Env::Env(Memory& m, RegisterCache& r) : mem_{m}, cache_{r} {}
void Env::insert_stack(const llvm::Value& key, const Domain& val) {
  mem_.insert_stack(Value::make(key), val);
}
void Env::insert_heap(const Symbol& key, const Domain& val) {
  mem_.insert_heap(key, val);
}

}  // namespace stacksafe
