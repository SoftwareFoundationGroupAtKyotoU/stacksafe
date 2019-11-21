#include "hash.hpp"
#include "key.hpp"
#include "map.hpp"
#include "symbol.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Key& key) {
  return llvm::hash_value(key.sym_);
}
llvm::hash_code hash_value(const Map& map) {
  return map.hash_;
}
llvm::hash_code hash_value(const Symbol& sym) {
  return hash_value(sym.key_);
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Symbol>::operator()(const stacksafe::Symbol& sym) const {
  return hash_value(sym);
}
}  // namespace std
