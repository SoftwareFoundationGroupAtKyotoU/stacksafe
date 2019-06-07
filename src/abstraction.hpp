#ifndef INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
#define INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961

#include "environment.hpp"
#include "location.hpp"
#include <unordered_map>
#include <unordered_set>

namespace llvm {
class BasicBlock;
class Function;
} // namespace llvm

namespace stacksafe {
class Abstraction {
  using BB = llvm::BasicBlock *;
  LocationFactory factory_;
  std::unordered_map<BB, Env_> map_;
  std::unordered_set<BB> todo_;

public:
  Abstraction(llvm::Function &F);
  void proceed();
  void update(BB b);
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
