#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <string>
#include <unordered_map>

namespace llvm {
class Function;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {
class Register;
class Symbol;

class Cache : private std::unordered_map<const llvm::Value*, int> {
  using Super = std::unordered_map<const llvm::Value*, int>;

 public:
  explicit Cache(const llvm::Function& f);
  std::string to_str(const Symbol& sym) const;
  std::string to_str(const Register& reg) const;
  std::string to_str(const llvm::Instruction& reg) const;

 private:
  int lookup(const llvm::Value* val) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
