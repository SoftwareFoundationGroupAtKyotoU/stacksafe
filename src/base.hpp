#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
class Value;
}  // namespace llvm

namespace stacksafe {

class Base {
  using Ptr = const void*;
  using Int = std::uintptr_t;
  union {
    Ptr reg_;
    Int sym_;
  };

 public:
  explicit Base(const llvm::Value& val);
  Base(Ptr val, bool is_local);
  Ptr ptr() const;
  Int val() const;
  bool is_symbol() const;
  bool is_register() const;
  bool is_local() const;
  bool is_global() const;
  static bool equals(const Base& lhs, const Base& rhs);
  static bool less(const Base& lhs, const Base& rhs);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
