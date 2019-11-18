#ifndef INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
#define INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485

#include <unordered_map>
#include "env.hpp"
#include "pool.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {

class Blocks : private std::unordered_map<const llvm::BasicBlock *, Env> {
  using Super = std::unordered_map<const llvm::BasicBlock *, Env>;
  MapPool &pool_;

 public:
  using Super::begin, Super::end;
  explicit Blocks(const llvm::Function &f, MapPool &pool);
  Env &get(const llvm::BasicBlock &b);
  void merge(const llvm::BasicBlock &dst, const FlatEnv &src);
  void merge(const llvm::BasicBlock &dst, const llvm::BasicBlock &src);
  FlatEnv concat(const llvm::BasicBlock &src);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
