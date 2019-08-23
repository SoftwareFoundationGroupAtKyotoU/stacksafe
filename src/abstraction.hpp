#ifndef INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
#define INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961

#include <unordered_map>
#include <unordered_set>
#include "environment.hpp"
#include "location.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Abstraction {
  using BB = llvm::BasicBlock *;
  LocationFactory factory_;
  std::unordered_map<BB, Env_> map_;
  std::unordered_set<BB> todo_;
  llvm::Function &func_;
  void initialize();
  void init_args();
  void update(BB b);

 public:
  Abstraction(llvm::Function &F);
};
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
