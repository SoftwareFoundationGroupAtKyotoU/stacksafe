#include "memory.hpp"
#include <llvm/IR/Function.h>
#include "cache.hpp"
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

Memory::Memory(const Cache &c) : cache_{c} {}
Memory::Memory(const Cache &c, const llvm::Function &f) : cache_{c} {
  insert(Register::get_global(), Domain::get_global());
  for (const auto &a : f.args()) {
    insert(a, Domain::get_global());
  }
}
const Map &Memory::heap() const {
  return heap_;
}
const Map &Memory::stack() const {
  return stack_;
}
bool Memory::includes(const Memory &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void Memory::merge(const Memory &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
const Domain &Memory::lookup(const Register &key) const {
  return heap_.lookup(key);
}
const Domain &Memory::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return stack_.lookup(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain::get_global();
  } else if (check_constant(key)) {
    return Domain::get_empty();
  }
  stacksafe_unreachable("neither register nor constant", key);
}
void Memory::insert(const Register &key, const Domain &val) {
  heap_.insert(key, val);
}
void Memory::insert(const llvm::Value &key, const Domain &val) {
  stack_.insert(cache_.lookup(key), val);
}
Register Memory::alloc(const llvm::Value &key) {
  const auto &reg = cache_.lookup(key);
  heap_.insert(reg, Domain::get_empty());
  stack_.insert(reg, Domain{reg});
  return reg;
}
void Memory::collect(const Register &curr, Domain &done) const {
  if (!done.includes(Domain{curr})) {
    done.merge(Domain{curr});
    for (const auto &next : lookup(curr)) {
      collect(next, done);
    }
  }
}
void to_json(Json &j, const Memory &x) {
  j["heap"] = x.heap();
  j["stack"] = x.stack();
}

}  // namespace stacksafe
