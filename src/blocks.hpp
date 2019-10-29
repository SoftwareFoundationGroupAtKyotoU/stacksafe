#ifndef INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
#define INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485

#include <map>
#include "memory.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Env;
class Fabric;
struct Log;

class Blocks : private std::map<const llvm::BasicBlock *, Memory> {
  using Super = std::map<const llvm::BasicBlock *, Memory>;
  Cache cache_;
  std::unique_ptr<Log> log_;

 public:
  using Super::begin, Super::end;
  explicit Blocks(const llvm::Function &f);
  ~Blocks();
  Memory interpret(const llvm::BasicBlock *b) const;
  bool update(const llvm::BasicBlock *b, const Memory &next);
  bool verify(const llvm::BasicBlock *b) const;
  void print(const llvm::BasicBlock *b, const Memory &next) const;
  void finish(const llvm::Function &f) const;
  Fabric diff(const llvm::BasicBlock *b, const Memory &next) const;

 private:
  Memory &get(const llvm::BasicBlock *b);
  const Memory &get(const llvm::BasicBlock *b) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_1C49E28A_FFB0_4196_8F04_3179A52F9485
