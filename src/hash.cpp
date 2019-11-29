#include "hash.hpp"
#include "value.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Value& val) {
  return llvm::hash_value(val.ptr());
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value& val) const {
  return hash_value(val);
}
}  // namespace std
