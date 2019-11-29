#ifndef INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE
#define INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE

#include <cstdint>

namespace llvm {
class AllocaInst;
}

namespace stacksafe {

class Symbol {
  std::uintptr_t sym_;

 public:
  explicit Symbol(const llvm::AllocaInst& i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FAE8D5CB_240E_4E65_B38A_B29E5FF091CE
