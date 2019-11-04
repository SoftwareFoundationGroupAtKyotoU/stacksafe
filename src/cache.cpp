#include "cache.hpp"
#include <llvm/IR/Function.h>
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
const Register &Cache::lookup(const llvm::Value &key) const {
  auto it = Super::find(&key);
  assert(Super::end() != it && "not registered in cache");
  return it->second;
}
void Cache::add(const llvm::Value &reg) {
  auto num = register_number(reg);
  assert(num && "not a register");
  Super::try_emplace(&reg, Register{*num});
}

}  // namespace stacksafe
