#ifndef INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
#define INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C

#include <memory>
#include <string>
#include <unordered_map>
#include "value.hpp"

namespace llvm {
class Function;
class Instruction;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Symbol;

class Cache : private std::unordered_map<Value, int> {
  using Super = std::unordered_map<Value, int>;

 public:
  explicit Cache(const llvm::Function& f);
  std::string to_str(const Symbol& sym) const;
  std::string to_str(const llvm::Instruction& reg) const;
  std::string to_str(const Domain& dom) const;

 private:
  static std::string to_str(int num);
  int lookup(const Value& val) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_09CD683A_2DAD_4D5C_A176_5CCF7268ED8C
