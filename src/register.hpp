#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>

namespace llvm {
class Value;
}

namespace stacksafe {

class Register {
  const llvm::Value *const reg_;
  explicit Register(const llvm::Value *v);

 public:
  int number() const;
  const llvm::Value *value() const;
  bool less(const Register &that) const;
  bool is_local() const;
  static const Register &get_global();
  static Register get_local(const llvm::Value &v);
};
bool operator<(const Register &lhs, const Register &rhs);
std::string to_str(const Register &reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
