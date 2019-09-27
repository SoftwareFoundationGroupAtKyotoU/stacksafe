#include "memory.hpp"
#include "json.hpp"

namespace stacksafe {

const Memory::Heap& Memory::heap() const {
  return heap_;
}
Memory::Heap& Memory::heap() {
  return heap_;
}
const Memory::Stack& Memory::stack() const {
  return stack_;
}
Memory::Stack& Memory::stack() {
  return stack_;
}
bool Memory::includes(const Memory& that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void Memory::merge(const Memory& that) {
  heap_.insert(that.heap_);
  stack_.insert(that.stack_);
}
void to_json(Json& j, const Memory& x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
