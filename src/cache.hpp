#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <map>
#include <memory>
#include <optional>
#include <string>

namespace llvm {
class Value;
}

namespace stacksafe {
class Register;

class Cache {
  using Super = std::map<const llvm::Value*, int>;
  std::unique_ptr<Super> cache_;
  int lookup(const llvm::Value* reg) const;
  std::string to_str(const llvm::Value* reg) const;

 public:
  Cache();
  std::string to_str(const Register& reg) const;
  std::string to_str(const llvm::Value& reg) const;

 private:
  static std::string to_str(int num);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
