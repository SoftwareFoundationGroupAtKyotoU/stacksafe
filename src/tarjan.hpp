#ifndef INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
#define INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8

#include <map>
#include <stack>
#include <vector>

namespace llvm {
class BasicBlock;
class Function;
class Value;
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
 protected:
  using BB = const llvm::Value*;
  using Vec = std::vector<BB>;
  std::map<BB, Frame> frames_;
  std::stack<BB> stack_;
  int index_;
  std::vector<Vec> result_;

 public:
  virtual ~Tarjan();
  Tarjan(const llvm::Function& f);
  const std::vector<Vec>& result() const;

 protected:
  virtual std::vector<BB> successors(BB b) const = 0;
  bool visit(BB b);
  Vec collect(BB b);
  Frame& push(BB b);
  BB pop();
};

class BlockSolver : public Tarjan {
 public:
  explicit BlockSolver(const llvm::Function& f);
  static std::vector<Blocks> scc(const llvm::Function& f);

 protected:
  std::vector<BB> successors(BB b) const override;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
