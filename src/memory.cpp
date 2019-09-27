#include "memory.hpp"
#include "json.hpp"

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
void to_json(Json& j, const Memory& x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
