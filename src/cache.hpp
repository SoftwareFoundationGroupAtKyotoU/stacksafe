#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <memory>
#include <string>
#include <unordered_map>

namespace llvm {
class Value;
}

namespace stacksafe {
class Domain;
class Register;

class Cache {
  using Super = std::unordered_map<const llvm::Value*, int>;
  std::unique_ptr<Super> cache_;

 public:
  Cache();
  std::string to_str(const Register& reg) const;
  std::string to_str(const llvm::Value& reg) const;
  std::string to_str(const Domain& dom) const;

 private:
  static std::string to_str(int num);
  int lookup(const llvm::Value* reg) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
