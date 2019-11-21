#include "hash.hpp"
#include "key.hpp"
#include "map.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Key& key) {
  return llvm::hash_value(key.sym_);
}
llvm::hash_code hash_value(const Map& map) {
  return map.hash_;
}

}  // namespace stacksafe
