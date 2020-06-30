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
  using Ptr = const llvm::Value*;
  using Vec = std::vector<Ptr>;

 public:
  virtual ~Tarjan() = 0;
  void run(const Vec& v);
  const std::vector<Vec>& result() const;

 private:
  virtual std::vector<Ptr> successors(Ptr b) const = 0;
  bool visit(Ptr b);
  Vec collect(Ptr b);
  Frame& push(Ptr b);
  Ptr pop();

  int index_ = 0;
  std::map<Ptr, Frame> frames_;
  std::stack<Ptr> stack_;
  std::vector<Vec> result_;
};

class BlockSolver : public Tarjan {
 public:
  static std::vector<Blocks> scc(const llvm::Function& f);

 protected:
  std::vector<Ptr> successors(Ptr b) const override;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
