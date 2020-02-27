#include "tarjan.hpp"
#include <llvm/IR/Function.h>
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

std::vector<Blocks> Tarjan::run(const llvm::Function& f) {
  std::vector<Blocks> vec;
  Tarjan tarjan{f};
  for (const auto& b : f) {
    tarjan.visit(&b, vec);
  }
  for (auto& c : vec) {
    std::reverse(c.begin(), c.end());
  }
  std::reverse(vec.begin(), vec.end());
  return vec;
}
Tarjan::Tarjan(const llvm::Function& f) : index_{0} {
  for (const auto& b : f) {
    frames_.try_emplace(&b);
  }
}
bool Tarjan::visit(BB b, std::vector<Blocks>& vec) {
  const auto ok = frames_[b].is_undef();
  if (ok) {
    auto& frame = push(b);
    for (const auto& succ : Blocks::successors(b)) {
      const auto& next = frames_[succ];
      if (visit(succ, vec)) {
        frame.update(next.low());
      } else if (next.on_stack()) {
        frame.update(next.index());
      }
    }
    if (frame.is_root()) {
      Blocks blocks;
      while (true) {
        const auto p = pop();
        blocks.push_back(p);
        if (b == p) {
          break;
        }
      }
      vec.emplace_back(blocks);
    }
  }
  return ok;
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

}  // namespace stacksafe
