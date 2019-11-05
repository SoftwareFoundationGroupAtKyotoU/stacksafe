#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <map>
#include <memory>
#include <optional>
#include <string>
#include "register.hpp"

namespace stacksafe {

class Cache {
  using Super = std::map<Register, int>;
  std::unique_ptr<Super> cache_;
  std::optional<int> lookup(const llvm::Value& reg) const;

 public:
  Cache();
  int lookup(const Register& reg);
  std::string to_str(const Register& reg) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
