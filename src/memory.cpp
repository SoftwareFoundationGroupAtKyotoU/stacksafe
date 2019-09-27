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
const Stack& Memory::stack() const {
  return stack_;
}
const RegMap& Memory::regmap() const {
  return regmap_;
}
bool Memory::includes(const Memory& that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_) &&
         regmap_.includes(that.regmap_);
}
void Memory::merge(const Memory& that) {
  heap_.insert(that.heap_);
  stack_.insert(that.stack_);
  regmap_.insert(that.regmap_);
}
void Memory::insert_stack(const Register& key, const Domain& val) {
  regmap_.insert(key, val);
}
void Memory::insert_heap(const Symbol& key, const Domain& val) {
  heap_.insert(key, val);
}
Domain Memory::from_stack(const Register& key) const {
  if (auto dom = regmap_.get(key)) {
    return *dom;
  }
  return Domain{};
}
Domain Memory::from_heap(const Symbol& sym) const {
  if (auto d = heap_.get(sym)) {
    return *d;
  }
  return Domain{};
}
void to_json(Json& j, const Memory& x) {
  j["stack"] = x.regmap();
  j["heap"] = x.heap();
}

}  // namespace stacksafe
