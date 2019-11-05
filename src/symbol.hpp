#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <functional>

namespace llvm {
class Value;
}

namespace stacksafe {

class Symbol {
  const llvm::Value *const val_;
  explicit Symbol(const llvm::Value *v);

 public:
  const llvm::Value *value() const;
  bool is_local() const;
  static const Symbol &get_global();
  static Symbol get_local(const llvm::Value &v);
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
bool operator==(const Symbol &lhs, const Symbol &rhs);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::Symbol> {
  size_t operator()(const stacksafe::Symbol &sym) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
