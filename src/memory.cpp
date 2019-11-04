#include "memory.hpp"
#include <llvm/IR/Function.h>
#include "fabric.hpp"
#include "json.hpp"
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Cache::Cache(const llvm::Function &f) {
  for (const auto &a : f.args()) {
    add(a);
  }
  for (const auto &b : f) {
    for (const auto &i : b) {
      if (check_register(i)) {
        add(i);
      }
    }
  }
}
Register Cache::lookup(const llvm::Value &key) const {
  auto it = Super::find(&key);
  assert(Super::end() != it && "not registered in cache");
  return it->second;
}
void Cache::add(const llvm::Value &reg) {
  auto num = register_number(reg);
  assert(check_register(reg) && num && "only registers are allowed");
  Super::try_emplace(&reg, *num);
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
