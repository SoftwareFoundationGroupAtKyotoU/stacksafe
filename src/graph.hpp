#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <vector>

namespace llvm {
class BasicBlock;
}

namespace stacksafe {

class Scc : private std::vector<const llvm::BasicBlock*> {
  using Super = std::vector<const llvm::BasicBlock*>;

 public:
  using Super::emplace_back;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
