#include "memory.hpp"
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

const Heap &Memory::heap() const {
  return heap_;
}
const Stack &Memory::stack() const {
  return stack_;
}
bool Memory::includes(const Memory &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void Memory::merge(const Memory &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
Fabric Memory::diff(const Memory &that) const {
  Fabric ret;
  ret.append(heap().diff(that.heap())).next();
  ret.append(stack().diff(that.stack())).next();
  return ret;
}
Domain Memory::lookup(const Symbol &key) const {
  return heap_.lookup(key);
}
Domain Memory::lookup(const Register &key) const {
  return stack_.lookup(key);
}
void Memory::insert(const Symbol &key, const Domain &val) {
  heap_.insert(key, val);
}
void Memory::insert(const Register &key, const Domain &val) {
  stack_.insert(key, val);
}
void to_json(Json &j, const Memory &x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
