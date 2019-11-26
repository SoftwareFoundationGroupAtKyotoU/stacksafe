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

class Block {
  const llvm::BasicBlock* block_;

 public:
  explicit Block(const llvm::BasicBlock& b);
  std::vector<Block> successors() const;
  const llvm::BasicBlock& get() const;
  bool operator==(const Block& block) const;
};

class Component : private std::vector<Block> {
  using Super = std::vector<Block>;
  Map map_;

 public:
  explicit Component(const std::vector<Block>& vec);
  bool contains(const Block& b) const;
  bool is_loop() const;
  std::vector<Block> out_degree() const;
  void merge(const Map& m);
};

class SCC : private std::vector<Component> {
  using Super = std::vector<Component>;

 public:
  explicit SCC(const llvm::Function& f);
  Component pop();
};

class Tarjan {
  std::vector<Frame> frames_;
  std::map<const llvm::BasicBlock*, Frame*> map_;
  std::stack<Block> stack_;
  std::vector<Scc> scc_;
  int index_;

 public:
  Tarjan(const llvm::Function& f);
  const std::vector<Scc>& scc() const;
  void visit(const Block& b);
  Frame& push(const Block& b);
  void update(Frame& frame, const Block& succ);
  Block pop();
  Scc collect(const Block& b);
  Frame& map(const Block& b);
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
