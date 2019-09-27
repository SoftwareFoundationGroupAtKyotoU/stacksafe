#ifndef INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
#define INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485

#include <map>
#include "cache.hpp"
#include "env.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
struct Log;

class Blocks : private std::map<const llvm::BasicBlock*, Memory> {
  using Super = std::map<const llvm::BasicBlock*, Memory>;
  RegisterCache cache_;

 public:
  using Super::begin, Super::end;
  explicit Blocks(const llvm::Function& f);
  Memory interpret(const llvm::BasicBlock* b);
  bool update(const llvm::BasicBlock* b, const Memory& next);
  bool verify(const llvm::BasicBlock* b);
  void print(Log& log, const llvm::BasicBlock* b, const Memory& next);

 private:
  Memory& get(const llvm::BasicBlock* b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
