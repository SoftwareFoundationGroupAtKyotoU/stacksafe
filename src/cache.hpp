#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <map>
#include "register.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class Cache : private std::map<const llvm::Value *, Register> {
  using Super = std::map<const llvm::Value *, Register>;

 public:
  Register lookup(const llvm::Value &key) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
