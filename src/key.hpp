#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
class Value;
}

namespace stacksafe {

class Key {
  using Base = std::uintptr_t;
  union {
    const llvm::Value* val_;
    Base sym_;
  };
  static const Base symbol_flag, global_flag;

 public:
  Key();
  explicit Key(const llvm::Value& val);
  Key(const llvm::Value& val, bool is_arg);
  bool is_symbol() const;
  bool is_register() const;
  bool is_local() const;
  bool is_global() const;
  bool is_argument() const;
  static bool equals(const Key& lhs, const Key& rhs);
  static bool less(const Key& lhs, const Key& rhs);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
