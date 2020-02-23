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

class Blocks : private std::vector<const llvm::BasicBlock*> {
  using Super = std::vector<const llvm::BasicBlock*>;
  using BB = Super::value_type;

 public:
  using Super::begin, Super::end, Super::push_back;
  using Super::value_type;
  static Blocks successors(BB b);
  Blocks successors() const;
  bool contains(BB b) const;
  bool is_loop() const;
};

class Components : private std::vector<std::tuple<Blocks, Map>> {
  using Pair = std::tuple<Blocks, Map>;
  using Super = std::vector<Pair>;
  using BB = Blocks::value_type;

 public:
  using Super::begin, Super::end;
  void reload();
  void append(BB b);
  Components& init(const llvm::Function& f);
  void transfer(const Blocks& b, const Map& pred);

 private:
  Map& find(BB b);
};

class Tarjan {
  using BB = Blocks::value_type;
  std::map<BB, Frame> frames_;
  std::stack<BB> stack_;
  Components comps_;
  int index_;

 public:
  Tarjan(const llvm::Function& f);
  bool visit(BB b);
  void update(Frame& prev, BB succ);
  Frame& push(BB b);
  BB pop();
  static Components run(const llvm::Function& f);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
