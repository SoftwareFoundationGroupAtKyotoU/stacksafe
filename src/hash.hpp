#ifndef INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
#define INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7

#include <llvm/ADT/Hashing.h>
#include <functional>

namespace stacksafe {
class Node;

llvm::hash_code hash_value(const Node& node);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::Node> {
  size_t operator()(const stacksafe::Node& node) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
