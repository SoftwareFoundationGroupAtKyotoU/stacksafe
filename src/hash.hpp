#ifndef INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
#define INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7

#include <llvm/ADT/Hashing.h>
#include <functional>

namespace stacksafe {
class Key;
class Map;
class MapRef;
class Symbol;

llvm::hash_code hash_value(const Key& key);
llvm::hash_code hash_value(const Map& map);
llvm::hash_code hash_value(const Symbol& sym);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapRef> {
  size_t operator()(const stacksafe::MapRef& r) const;
};
template <>
struct hash<stacksafe::Symbol> {
  size_t operator()(const stacksafe::Symbol& sym) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
