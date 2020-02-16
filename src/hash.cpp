#include "hash.hpp"
#include "node.hpp"

namespace stacksafe {

llvm::hash_code hash_value(const Node& node) {
  return llvm::hash_combine(node.index(), node.value());
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Node>::operator()(const stacksafe::Node& node) const {
  return hash_value(node);
}
}  // namespace std
