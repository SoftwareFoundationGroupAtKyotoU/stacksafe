#include "cache.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

Register Cache::lookup(const llvm::Value &key) const {
  return Register::get_local(key);
}

}  // namespace stacksafe
