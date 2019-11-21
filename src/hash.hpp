#ifndef INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
#define INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7

#include <llvm/ADT/Hashing.h>

namespace stacksafe {
class Key;
class Map;

llvm::hash_code hash_value(const Key& key);
llvm::hash_code hash_value(const Map& map);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
