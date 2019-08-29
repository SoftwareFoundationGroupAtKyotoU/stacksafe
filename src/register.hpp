#ifndef INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
#define INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17

#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class Register {
  int num_;
  const llvm::Value& val_;
  Type type_;
  std::string repr_;

 public:
  explicit Register(const llvm::Value& v);
  int get_num() const;
  const llvm::Value& get_val() const;
  const Type& get_type() const;
  bool is_register() const;
};
bool operator<(const Register& lhs, const Register& rhs);
void to_json(Json& j, const Register& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
