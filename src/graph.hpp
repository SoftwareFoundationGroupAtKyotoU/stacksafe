#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <map>
#include <stack>
#include <vector>
#include "map.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {

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

class Blocks;
class Component : private std::vector<Block> {
  using Super = std::vector<Block>;
  Map map_;

 public:
  using Super::begin, Super::end;
  explicit Component(const Blocks& vec);
  explicit Component(const std::vector<const llvm::BasicBlock*>& vec);
  bool contains(const Block& b) const;
  bool is_loop() const;
  std::vector<Block> successors() const;
  void merge(const Component& c);
  Map& map();
};

class Blocks : private std::vector<const llvm::BasicBlock*> {
  using BB = const llvm::BasicBlock*;
  using Super = std::vector<const llvm::BasicBlock*>;

 public:
  using Super::begin, Super::end, Super::push_back;
  static Blocks successors(BB b);
  Blocks successors() const;
  bool contains(BB b) const;
};

class Components : private std::vector<Blocks> {
  using Super = std::vector<Blocks>;

 public:
  using Super::push_back;
};

class Scc : private std::vector<Component> {
  using Super = std::vector<Component>;

 public:
  using Super::empty;
  explicit Scc(const llvm::Function& f);
  Component pop();
  Component& find(const Block& b);
  void distribute(const Component& c);
};

class Tarjan {
  using BB = const llvm::BasicBlock*;
  std::map<BB, Frame> frames_;
  std::stack<BB> stack_;
  std::vector<Component> scc_;
  Components comps_;
  int index_;

 public:
  Tarjan(const llvm::Function& f);
  const std::vector<Component>& scc() const;
  void visit(BB b);
  void update(Frame& prev, BB succ);
  Blocks collect(BB b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
