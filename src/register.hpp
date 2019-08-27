#ifndef INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
#define INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17

#include <string>
#include "json.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class Register {
  int num_;
  std::string repr_;

 public:
  explicit Register(const llvm::Value& v);
  int get_num() const;
  bool compare(const Register& that) const;
  std::string to_str() const;
};
bool operator<(const Register& lhs, const Register& rhs);
void to_json(Json& j, const Register& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
