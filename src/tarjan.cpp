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
auto Tarjan::result() const -> const std::vector<Vec>& {
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
auto Tarjan::collect(BB b) -> Vec {
  Vec blocks;
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
  const auto& result = tarjan->result();
  std::vector<Blocks> vec;
  for (auto it = result.crbegin(); it != result.crend(); ++it) {
    Blocks blocks;
    for (auto ptr = it->crbegin(); ptr != it->crend(); ++ptr) {
      if (auto b = llvm::dyn_cast<llvm::BasicBlock>(*ptr)) {
        blocks.push_back(b);
      }
    }
    vec.push_back(blocks);
  }
  return vec;
}
auto BlockSolver::successors(BB p) const -> std::vector<BB> {
  std::vector<BB> v;
  if (auto b = llvm::dyn_cast<llvm::BasicBlock>(p)) {
    const auto t = b->getTerminator();
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      v.push_back(t->getSuccessor(i));
    }
  }
  return v;
}

}  // namespace stacksafe
