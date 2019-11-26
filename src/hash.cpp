#include "hash.hpp"
#include "base.hpp"
#include "map.hpp"
#include "value.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Base& key) {
  return llvm::hash_value(key.sym_);
}
llvm::hash_code hash_value(const Value& sym) {
  return hash_value(static_cast<const Base&>(sym));
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value& sym) const {
  return hash_value(sym);
}
}  // namespace std
