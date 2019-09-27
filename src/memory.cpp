#include "memory.hpp"
#include <llvm/IR/Function.h>
#include "domain.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

const Heap& Memory::heap() const {
  return heap_;
}
Heap& Memory::heap() {
  return heap_;
}
const RegMap& Memory::stack() const {
  return regmap_;
}
RegMap& Memory::stack() {
  return regmap_;
}
bool Memory::includes(const Memory& that) const {
  return heap_.includes(that.heap_) && regmap_.includes(that.regmap_);
}
void Memory::merge(const Memory& that) {
  heap_.insert(that.heap_);
  regmap_.insert(that.regmap_);
}
void Memory::insert_stack(const Register& key, const Domain& val) {
  regmap_.insert(key, val);
}
void Memory::insert_heap(const Symbol& key, const Domain& val) {
  heap_.insert(key, val);
}
void to_json(Json& j, const Memory& x) {
  j["stack"] = x.stack();
  j["heap"] = x.heap();
}

}  // namespace stacksafe
