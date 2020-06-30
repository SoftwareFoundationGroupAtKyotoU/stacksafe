#include "tarjan.hpp"
#include <llvm/IR/Function.h>
#include <memory>
#include "block.hpp"

namespace stacksafe {

Frame::Frame() : index_{-1}, low_{-1}, on_stack_{false} {}
bool Frame::on_stack() const {
  return on_stack_;
}
int Frame::index() const {
  return index_;
}
int Frame::low() const {
  return low_;
}
bool Frame::is_undef() const {
  return index_ < 0;
}
bool Frame::is_root() const {
  return index_ == low_;
}
void Frame::push(int n) {
  index_ = low_ = n;
  on_stack_ = true;
}
void Frame::update(int n) {
  if (n < low_) {
    low_ = n;
  }
}
void Frame::pop() {
  on_stack_ = false;
}

Tarjan::~Tarjan() = default;
Tarjan::Tarjan(const llvm::Function& f) : index_{0} {
  for (const auto& b : f) {
    frames_.try_emplace(&b);
  }
}
const std::vector<Blocks>& Tarjan::result() const {
  return result_;
}
bool Tarjan::visit(BB b) {
  const auto ok = frames_[b].is_undef();
  if (ok) {
    auto& frame = push(b);
    for (const auto& succ : successors(b)) {
      const auto& next = frames_[succ];
      if (visit(succ)) {
        frame.update(next.low());
      } else if (next.on_stack()) {
        frame.update(next.index());
      }
    }
    if (frame.is_root()) {
      result_.push_back(collect(b));
    }
  }
  return ok;
}
Blocks Tarjan::collect(BB b) {
  Blocks blocks;
  BB p = nullptr;
  while (b != p) {
    p = pop();
    blocks.push_back(p);
  }
  return blocks;
}
Frame& Tarjan::push(BB b) {
  stack_.push(b);
  frames_[b].push(index_++);
  return frames_[b];
}
auto Tarjan::pop() -> BB {
  const auto b = stack_.top();
  stack_.pop();
  frames_[b].pop();
  return b;
}

BlockSolver::BlockSolver(const llvm::Function& f) : Tarjan{f} {}
std::vector<Blocks> BlockSolver::scc(const llvm::Function& f) {
  auto tarjan = std::make_unique<BlockSolver>(f);
  for (const auto& b : f) {
    tarjan->visit(&b);
  }
  std::vector<Blocks> vec{tarjan->result()};
  for (auto& c : vec) {
    std::reverse(c.begin(), c.end());
  }
  std::reverse(vec.begin(), vec.end());
  return vec;
}
auto BlockSolver::successors(BB b) const -> std::vector<BB> {
  std::vector<BB> v;
  const auto t = b->getTerminator();
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    v.push_back(t->getSuccessor(i));
  }
  return v;
}

}  // namespace stacksafe
