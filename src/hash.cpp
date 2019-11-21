#include "hash.hpp"
#include <llvm/ADT/Hashing.h>
#include "key.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Key& key) {
  return llvm::hash_value(key.sym_);
}

}  // namespace stacksafe
