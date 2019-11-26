#include "graph.hpp"
#include <llvm/IR/Function.h>
#include <map>
#include <stack>

namespace stacksafe {
namespace {
struct Frame {
  int index, low;
  bool on;
  Frame();
  bool undefined() const;
  bool is_root() const;
  void push(int i);
  void update(int n);
  void pop();
};
Frame::Frame() : index{-1}, low{-1}, on{false} {}
bool Frame::undefined() const {
  return index < 0;
}
bool Frame::is_root() const {
  return index == low;
}
void Frame::push(int i) {
  index = low = i;
  on = true;
}
void Frame::update(int n) {
  if (n < low) {
    low = n;
  }
}
void Frame::pop() {
  on = false;
}

class Tarjan {
  using BB = const llvm::BasicBlock*;
  std::vector<Frame> frames_;
  std::map<BB, Frame*> map_;
  std::stack<BB> stack_;
  std::vector<Scc> scc_;
  int index_;

 public:
  Tarjan(const llvm::Function& f);
  const std::vector<Scc> scc() const;
  void visit(BB b);
  Frame& push(BB b);
  void update(Frame& frame, BB succ);
  BB pop();
  Scc collect(BB b);
};
Tarjan::Tarjan(const llvm::Function& f) : frames_{f.size()}, index_{0} {
  std::size_t i = 0;
  for (const auto& b : f) {
    map_.try_emplace(&b, &frames_[i]);
    ++i;
  }
  for (const auto& b : f) {
    if (map_[&b]->undefined()) {
      visit(&b);
    }
  }
}
const std::vector<Scc> Tarjan::scc() const {
  return scc_;
}
void Tarjan::visit(BB b) {
  Frame& frame = push(b);
  const auto t = b->getTerminator();
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    const auto succ = t->getSuccessor(i);
    update(frame, succ);
  }
  if (frame.is_root()) {
    scc_.emplace_back(collect(b));
  }
}
Frame& Tarjan::push(BB b) {
  Frame& frame = *map_[b];
  frame.push(index_);
  ++index_;
  stack_.push(b);
  return frame;
}
void Tarjan::update(Frame& frame, BB succ) {
  const Frame& f = *map_[succ];
  if (f.undefined()) {
    visit(succ);
    frame.update(f.low);
  } else if (f.on) {
    frame.update(f.index);
  }
}
auto Tarjan::pop() -> BB {
  const auto b = stack_.top();
  stack_.pop();
  map_[b]->pop();
  return b;
}
Scc Tarjan::collect(BB b) {
  Scc scc;
  while (true) {
    const auto p = pop();
    scc.emplace_back(p);
    if (b == p) {
      break;
    }
  }
  std::reverse(scc.begin(), scc.end());
  return scc;
}
}  // namespace

bool Scc::contains(const llvm::BasicBlock* b) const {
  return std::find(begin(), end(), b) != end();
}
auto Scc::out_degree() const -> Set {
  Set out;
  for (const auto& b : *this) {
    const auto t = b->getTerminator();
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      const auto succ = t->getSuccessor(i);
      if (!contains(succ)) {
        out.emplace(succ);
      }
    }
  }
  return out;
}
void Scc::add_successor(const SccPtr& ptr) {
  succ_.emplace(ptr);
}

std::vector<Scc> strongly_connected(const llvm::Function& f) {
  Tarjan tarjan{f};
  auto scc = tarjan.scc();
  std::reverse(scc.begin(), scc.end());
  return scc;
}

}  // namespace stacksafe
