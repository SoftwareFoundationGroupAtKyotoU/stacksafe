#ifndef INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
#define INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961

#include "environment.hpp"
#include "location.hpp"
#include <queue>
#include <unordered_map>

namespace llvm {
  class BasicBlock;
  class Function;
}
namespace stacksafe {
  class Interpretation {
    using BB = llvm::BasicBlock *;
    LocationFactory factory_;
    std::unordered_map<BB, Env> map_;
    std::queue<BB> todo_;
  public:
    Interpretation(llvm::Function &F);
    void proceed();
    void update(BB b);
  };
}

#endif//INCLUDE_GUARD_FBC5E6D0_9143_4FDB_9ED6_5F3DE4E64961
