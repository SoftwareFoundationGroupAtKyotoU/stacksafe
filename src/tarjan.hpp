#ifndef INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
#define INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8

#include <map>
#include <stack>
#include <vector>

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Blocks;

class Frame {
  int index_, low_;
  bool on_stack_;

 public:
  Frame();
  bool on_stack() const;
  int index() const;
  int low() const;
  bool is_undef() const;
  bool is_root() const;
  void push(int n);
  void update(int n);
  void pop();
};

class Tarjan {
  using BB = const llvm::BasicBlock*;
  std::map<BB, Frame> frames_;
  std::stack<BB> stack_;
  int index_;

 public:
  static std::vector<Blocks> run(const llvm::Function& f);

 private:
  Tarjan(const llvm::Function& f);
  bool visit(BB b, std::vector<Blocks>& vec);
  Frame& push(BB b);
  BB pop();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
