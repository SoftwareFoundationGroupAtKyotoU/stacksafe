#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <memory>
#include <set>
#include <stack>
#include <vector>

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Scc;
using SccPtr = std::shared_ptr<Scc>;

class Scc : private std::vector<const llvm::BasicBlock*> {
  using Super = std::vector<const llvm::BasicBlock*>;
  using Set = std::set<const llvm::BasicBlock*>;
  std::set<SccPtr> succ_;

 public:
  using Stack = std::stack<SccPtr, std::vector<SccPtr>>;
  using Super::begin, Super::end, Super::emplace_back;
  bool contains(const llvm::BasicBlock* b) const;
  bool is_loop() const;
  Set out_degree() const;
  void add_successor(const SccPtr& ptr);
  static Stack decompose(const llvm::Function& f);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
