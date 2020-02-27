#ifndef INCLUDE_GUARD_25E5557B_880B_4E38_B068_6B33D855AA6E
#define INCLUDE_GUARD_25E5557B_880B_4E38_B068_6B33D855AA6E

#include "state.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {

class DataFlow {
  const llvm::Function& func_;
  State state_;

 public:
  explicit DataFlow(const llvm::Function& f);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_25E5557B_880B_4E38_B068_6B33D855AA6E
