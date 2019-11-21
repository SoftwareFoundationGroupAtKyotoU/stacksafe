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

 public:
  Key();
  explicit Key(const llvm::Value& val);
  Key(const llvm::Value& val, bool is_arg);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
