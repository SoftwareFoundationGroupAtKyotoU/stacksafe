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
bool Memory::merge(const Memory& that) {
  bool ret = false;
  if (heap_.merge(that.heap_)) {
    ret = true;
  }
  if (stack_.merge(that.stack_)) {
    ret = true;
  }
  return ret;
}
void to_json(Json& j, const Memory& x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
