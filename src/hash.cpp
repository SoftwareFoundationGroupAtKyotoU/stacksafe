#include "hash.hpp"
#include "map.hpp"
#include "value.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Value& val) {
  return llvm::hash_value(val.ptr());
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value& sym) const {
  return hash_value(sym);
}
}  // namespace std
