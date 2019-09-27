#ifndef INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
#define INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016

#include <set>
#include <vector>

namespace llvm {
class Value;
}

namespace stacksafe {
class Env;
class Memory;
class Value;

using ValueSet = std::set<const llvm::Value*>;

namespace instr {

void constant(Env& e, const llvm::Value& dst);

}  // namespace instr
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
