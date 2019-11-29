#ifndef INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE
#define INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE

#include <cstdint>

namespace llvm {
class AllocaInst;
class Argument;
class Value;
}  // namespace llvm

namespace stacksafe {

class Symbol {
  std::uintptr_t sym_;
  explicit Symbol(const llvm::Value* p);

 public:
  explicit Symbol(const llvm::AllocaInst& i);
  explicit Symbol(const llvm::Argument& a);
  const llvm::Value* value() const;
  std::uintptr_t sym() const;
  bool is_global() const;
  bool is_local() const;
  const llvm::Argument* as_argument() const;
  static Symbol get_global();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE
