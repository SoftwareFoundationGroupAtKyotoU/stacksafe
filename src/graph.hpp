#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <map>
#include <memory>
#include <set>
#include <stack>
#include <vector>
#include "map.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Scc;
using SccPtr = std::shared_ptr<Scc>;

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
  std::vector<Frame> frames_;
  std::map<BB, Frame*> map_;
  std::stack<BB> stack_;
  std::vector<Scc> scc_;
  int index_;

 public:
  Tarjan(const llvm::Function& f);
  const std::vector<Scc>& scc() const;
  void visit(BB b);
  Frame& push(BB b);
  void update(Frame& frame, BB succ);
  BB pop();
  Scc collect(BB b);
};

class Scc : private std::vector<const llvm::BasicBlock*> {
  using Super = std::vector<const llvm::BasicBlock*>;
  using Set = std::set<const llvm::BasicBlock*>;
  std::set<SccPtr> succ_;
  Map map_;

 public:
  using Stack = std::stack<SccPtr, std::vector<SccPtr>>;
  using Super::begin, Super::end, Super::emplace_back;
  bool contains(const llvm::BasicBlock* b) const;
  bool is_loop() const;
  Set out_degree() const;
  void add_successor(const SccPtr& ptr);
  void merge(const Map& map);
  void convey();
  Map& map();
  static Stack decompose(const llvm::Function& f);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
