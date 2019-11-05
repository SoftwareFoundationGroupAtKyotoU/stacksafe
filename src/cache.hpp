#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <map>
#include <memory>
#include <string>
#include "register.hpp"

namespace stacksafe {

class Cache : private std::map<Register, int> {
  using Super = std::map<Register, int>;
  std::unique_ptr<Super> cache_;

 public:
  int lookup(const Register& reg);
  std::string to_str(const Register& reg);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
