#ifndef INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
#define INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485

#include <unordered_map>
#include "env.hpp"
#include "flat.hpp"
#include "pool.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {

class Blocks : private std::unordered_map<const llvm::BasicBlock *, Env> {
  using Super = std::unordered_map<const llvm::BasicBlock *, Env>;
  MapPool pool_;
  FlatMapPool flat_;

 public:
  using Super::begin, Super::end;
  explicit Blocks(const llvm::Function &f);
  void merge(const llvm::BasicBlock &dst, const FlatEnv &src);
  void merge(const llvm::BasicBlock &dst, const llvm::BasicBlock &src);
  FlatEnvOld concat(const llvm::BasicBlock &src);

 private:
  Env &get(const llvm::BasicBlock &b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
