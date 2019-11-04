#include "memory.hpp"
#include <llvm/IR/Function.h>
#include "cache.hpp"
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

Memory::Memory(const Cache &c) : cache_{c} {}
Memory::Memory(const Cache &c, const llvm::Function &f) : cache_{c} {
  insert(Symbol::global(), Domain::global());
  for (const auto &a : f.args()) {
    insert(cache_.lookup(a), Domain::global());
  }
}
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
Domain Memory::lookup(const Symbol &key) const {
  return heap_.lookup(key);
}
Domain Memory::lookup(const Register &key) const {
  return stack_.lookup(key);
}
Domain Memory::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return stack_.lookup(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain::global();
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
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
