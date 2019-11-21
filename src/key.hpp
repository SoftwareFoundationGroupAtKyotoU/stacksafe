#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
class Value;
}

namespace stacksafe {

class Key {
  union {
    const llvm::Value* val_;
    std::uintptr_t sym_;
  };
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
