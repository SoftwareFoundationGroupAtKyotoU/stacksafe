#ifndef INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
#define INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7

namespace llvm {
class hash_code;
}

namespace stacksafe {
class Key;

llvm::hash_code hash_value(const Key& key);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_041B1474_160B_42A1_8A97_CA2CB3338CA7
