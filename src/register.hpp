#ifndef INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
#define INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17

#include <string>

namespace llvm {
class Value;
}

class Register {
  int num_;
  std::string repr_;

 public:
  explicit Register(const llvm::Value& v);
  bool compare(const Register& that) const;
};
bool operator<(const Register& lhs, const Register& rhs);

#endif  // INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
