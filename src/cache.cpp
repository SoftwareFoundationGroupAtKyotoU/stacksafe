#include "cache.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

Cache::Cache(const llvm::Function &f) {
  for (const auto &a : f.args()) {
    Super::try_emplace(&a, Register::get_local(a));
  }
  for (const auto &b : f) {
    for (const auto &i : b) {
      if (check_register(i)) {
        Super::try_emplace(&i, Register::get_local(i));
      }
    }
  }
}
const Register &Cache::lookup(const llvm::Value &key) const {
  auto it = Super::find(&key);
  assert(Super::end() != it && "not registered in cache");
  return it->second;
}

}  // namespace stacksafe
