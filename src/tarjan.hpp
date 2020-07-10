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
namespace tarjan {

class Solver {
  struct Frame;

 protected:
  using Ptr = const llvm::Value *;
  using Vec = std::vector<Ptr>;

 public:
  virtual ~Solver() = 0;
  void push_back(Ptr p);
  void run();
  const std::vector<Vec> &result() const;

 private:
  virtual std::vector<Ptr> successors(Ptr b) const = 0;
  bool visit(Ptr b);
  Vec collect(Ptr b);
  Frame &push(Ptr b);
  Ptr pop();

  int index_ = 0;
  Vec init_;
  std::map<Ptr, Frame> frames_;
  std::stack<Ptr> stack_;
  std::vector<Vec> result_;

  struct Frame {
    int index = -1, low = -1;
    bool on_stack = false;
    bool is_undef() const;
    bool is_root() const;
    void update(int n);
    void push(int n);
    void pop();
  };
};

class BlockSolver : public Solver {
 public:
  static std::vector<Blocks> scc(const llvm::Function &f);

 protected:
  std::vector<Ptr> successors(Ptr b) const override;
};

}  // namespace tarjan
}  // namespace stacksafe

namespace dataflow {
struct Frame {
  int index = -1, low = -1;
  bool on_stack = false;
  bool is_undef() const;
  bool is_root() const;
  void update(int n);
  void push(int n);
  void pop();
};
}  // namespace dataflow

#endif  // INCLUDE_GUARD_BA07B9AE_49AB_49BE_AE38_7CF6DF2C84C8
