#include "memory.hpp"
#include "json.hpp"
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Register Cache::lookup(const llvm::Value& key) {
  if (auto it = Super::find(&key); Super::end() != it) {
    return it->second;
  } else if (auto num = register_number(key)) {
    Register reg{*num};
    Super::try_emplace(&key, reg);
    return reg;
  } else {
    stacksafe_unreachable("only registers are allowed", key);
  }
}
void Cache::add(const llvm::Value& reg) {
  auto num = register_number(reg);
  assert(check_register(reg) && num && "only registers are allowed");
  Super::try_emplace(&reg, *num);
}

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
