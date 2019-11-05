#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>

namespace llvm {
class Value;
}

namespace stacksafe {

class Register {
  const llvm::Value *const reg_;
  friend class Cache;
  Register();
  explicit Register(const llvm::Value &v);

 public:
  int number() const;
  bool less(const Register &that) const;
  bool is_local() const;
  static const Register &get_global();
};
bool operator<(const Register &lhs, const Register &rhs);
std::string to_str(const Register &reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
