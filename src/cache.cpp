#include "cache.hpp"
#include "utility.hpp"

namespace stacksafe {

Register RegisterCache::lookup(const llvm::Value& key) {
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

}  // namespace stacksafe
