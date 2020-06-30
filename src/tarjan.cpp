#include "tarjan.hpp"
#include <llvm/IR/Function.h>
#include "block.hpp"

namespace stacksafe {

Tarjan::~Tarjan() = default;
auto Tarjan::result() const -> const std::vector<Vec>& {
  return result_;
}
void Tarjan::run(const Vec& v) {
  for (const auto& p : v) {
    frames_.try_emplace(p);
  }
  for (const auto& p : v) {
    visit(p);
  }
}
bool Tarjan::visit(Ptr p) {
  const auto ok = frames_[p].is_undef();
  if (ok) {
    auto& frame = push(p);
    for (const auto& succ : successors(p)) {
      const auto& next = frames_[succ];
      if (visit(succ)) {
        frame.update(next.low);
      } else if (next.on_stack) {
        frame.update(next.index);
      }
    }
    if (frame.is_root()) {
      result_.push_back(collect(p));
    }
  }
  return ok;
}
auto Tarjan::collect(Ptr p) -> Vec {
  Vec blocks;
  Ptr q = nullptr;
  while (p != q) {
    q = pop();
    blocks.push_back(q);
  }
  return blocks;
}
auto Tarjan::push(Ptr p) -> Frame& {
  stack_.push(p);
  frames_[p].push(index_++);
  return frames_[p];
}
auto Tarjan::pop() -> Ptr {
  const auto p = stack_.top();
  stack_.pop();
  frames_[p].pop();
  return p;
}

bool Tarjan::Frame::is_undef() const {
  return index < 0;
}
bool Tarjan::Frame::is_root() const {
  return index == low;
}
void Tarjan::Frame::update(int n) {
  if (n < low) {
    low = n;
  }
}
void Tarjan::Frame::push(int n) {
  index = low = n;
  on_stack = true;
}
void Tarjan::Frame::pop() {
  on_stack = false;
}

std::vector<Blocks> BlockSolver::scc(const llvm::Function& f) {
  auto tarjan = std::make_unique<BlockSolver>();
  Vec init;
  for (const auto& b : f) {
    init.push_back(&b);
  }
  tarjan->run(init);
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
auto BlockSolver::successors(Ptr p) const -> std::vector<Ptr> {
  std::vector<Ptr> v;
  if (auto b = llvm::dyn_cast<llvm::BasicBlock>(p)) {
    const auto t = b->getTerminator();
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      v.push_back(t->getSuccessor(i));
    }
  }
  return v;
}

}  // namespace stacksafe
