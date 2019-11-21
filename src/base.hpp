#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
class Value;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Base {
  using Ptr = const void*;
  using Int = std::uintptr_t;
  union {
    Ptr val_;
    Int sym_;
  };

 public:
  explicit Base(const llvm::Value& val);
  Base(Ptr val, bool is_arg);
  const llvm::Value* value() const;
  bool is_symbol() const;
  bool is_register() const;
  bool is_local() const;
  bool is_global() const;
  static bool equals(const Base& lhs, const Base& rhs);
  static bool less(const Base& lhs, const Base& rhs);
  friend llvm::hash_code hash_value(const Base& key);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
