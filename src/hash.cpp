#include "hash.hpp"
#include "node.hpp"
#include "value.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Value& val) {
  return llvm::hash_value(val.ptr());
}
llvm::hash_code hash_value(const Node& node) {
  return llvm::hash_combine(node.index(), node.value());
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value& val) const {
  return hash_value(val);
}
size_t hash<stacksafe::Node>::operator()(const stacksafe::Node& node) const {
  return hash_value(node);
}
}  // namespace std
