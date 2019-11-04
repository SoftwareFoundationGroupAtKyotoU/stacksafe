#ifndef INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
#define INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485

#include <map>
#include <optional>
#include "cache.hpp"
#include "env.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Log;

class Blocks : private std::map<const llvm::BasicBlock *, Env> {
  using Super = std::map<const llvm::BasicBlock *, Env>;
  Cache cache_;
  const Log &log_;

 public:
  using Super::begin, Super::end;
  Blocks(const llvm::Function &f, const Log &l);
  std::optional<Env> interpret(const llvm::BasicBlock &b);
  bool update(const llvm::BasicBlock &b, const Env &next);

 private:
  Env &get(const llvm::BasicBlock &b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
