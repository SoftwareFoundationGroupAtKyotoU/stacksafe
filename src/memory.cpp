#include "memory.hpp"
#include <llvm/IR/Function.h>
#include "domain.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Memory::Memory(const llvm::Function& f) {
  auto g = Symbol::global();
  insert_heap(g, Domain{g});
  for (auto& a : f.args()) {
    insert_stack(Value::make(a), Domain{g});
  }
}
const Heap& Memory::heap() const { return heap_; }
const Stack& Memory::stack() const { return stack_; }
bool Memory::includes(const Memory& that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_) &&
         regmap_.includes(that.regmap_);
}
void Memory::merge(const Memory& that) {
  heap_.insert(that.heap_);
  stack_.insert(that.stack_);
  regmap_.insert(that.regmap_);
}
void Memory::insert_stack(const Value& key, const Domain& val) {
  assert(key.is_register() && "insert to non-register");
  stack_.insert(key, val);
}
void Memory::insert_heap(const Symbol& key, const Domain& val) {
  heap_.insert(key, val);
}
Domain Memory::from_stack(const Value& reg) const {
  if (reg.is_register()) {
    // comparison on only registers is allowed
    if (auto d = stack_.get(reg)) {
      return *d;
    }
  }
  return reg.get_domain();
}
Domain Memory::from_heap(const Symbol& sym) const {
  if (auto d = heap_.get(sym)) {
    return *d;
  }
  return Domain{};
}
void Memory::call(const Domain& dom) {
  for (auto& sym : dom) {
    insert_heap(sym, dom);
  }
}
void to_json(Json& j, const Memory& x) {
  j["stack"] = x.stack();
  j["heap"] = x.heap();
}

}  // namespace stacksafe
