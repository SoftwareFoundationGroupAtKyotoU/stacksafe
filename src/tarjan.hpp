#ifndef INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
#define INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8

#include <map>
#include <stack>
#include "graph.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {
class Components;

class Tarjan {
  using BB = Blocks::value_type;
  std::map<BB, Frame> frames_;
  std::stack<BB> stack_;
  int index_;

 public:
  static Components run(const llvm::Function& f);

 private:
  Tarjan(const llvm::Function& f);
  bool visit(BB b, Components& comps);
  Frame& push(BB b);
  BB pop();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
