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

const Memory::Heap &Memory::heap() const {
  return heap_;
}
Memory::Heap &Memory::heap() {
  return heap_;
}
const Memory::Stack &Memory::stack() const {
  return stack_;
}
Memory::Stack &Memory::stack() {
  return stack_;
}
void Memory::init_arg(const Register &reg) {
  stack_.insert(reg, Domain::global());
}
void Memory::init_reg(const Register &reg) {
  stack_.insert(reg, Domain{});
}
bool Memory::includes(const Memory &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
bool Memory::merge(const Memory &that) {
  bool ret = false;
  if (heap_.merge(that.heap_)) {
    ret = true;
  }
  if (stack_.merge(that.stack_)) {
    ret = true;
  }
  return ret;
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
