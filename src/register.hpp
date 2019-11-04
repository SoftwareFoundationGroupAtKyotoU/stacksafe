#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <map>
#include <string>
#include "number.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class Register : private Number {
  friend class Cache;
  explicit Register(int n);

 public:
  const Number &number() const;
};
std::string to_str(const Register &reg);

class Cache : private std::map<const llvm::Value *, Register> {
  using Super = std::map<const llvm::Value *, Register>;

 public:
  explicit Cache(const llvm::Function &f);
  Register lookup(const llvm::Value &key) const;

 private:
  void add(const llvm::Value &reg);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
