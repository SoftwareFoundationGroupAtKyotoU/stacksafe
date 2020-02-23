#include "graph.hpp"
#include <llvm/IR/Function.h>

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

Blocks Blocks::successors(BB b) {
  Blocks succs;
  const auto t = b->getTerminator();
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    succs.push_back(t->getSuccessor(i));
  }
  return succs;
}
Blocks Blocks::successors() const {
  Blocks succs;
  for (const auto& b : *this) {
    for (const auto& succ : successors(b)) {
      if (!contains(succ) && !succs.contains(succ)) {
        succs.push_back(succ);
      }
    }
  }
  return succs;
}
bool Blocks::contains(BB b) const {
  return std::find(begin(), end(), b) != end();
}
bool Blocks::is_loop() const {
  for (const auto& b : *this) {
    for (const auto& succ : successors(b)) {
      if (contains(succ)) {
        return true;
      }
    }
  }
  return false;
}

void Components::push(const Blocks& b) {
  Super::emplace_back(b, Map{});
}
void Components::init(const llvm::Function& f) {
  find(&f.getEntryBlock()).init(f);
}
Map& Components::find(BB b) {
  const auto pred = [b](const Pair& pair) {
    return std::get<0>(pair).contains(b);
  };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return std::get<1>(*it);
}
void Components::transfer(const Blocks& b, const Map& pred) {
  for (const auto& succ : b.successors()) {
    find(succ).merge(pred);
  }
}

Tarjan::Tarjan(const llvm::Function& f) : index_{0} {
  for (const auto& b : f) {
    frames_.try_emplace(&b);
  }
  for (const auto& b : f) {
    if (frames_[&b].is_undef()) {
      visit(&b);
    }
  }
  std::reverse(comps_.begin(), comps_.end());
}
void Tarjan::visit(BB b) {
  stack_.push(b);
  auto& frame = frames_[b];
  frame.push(index_++);
  for (const auto& succ : Blocks::successors(b)) {
    update(frame, succ);
  }
  if (frame.is_root()) {
    comps_.push(collect(b));
  }
}
void Tarjan::update(Frame& prev, BB succ) {
  const auto& next = frames_[succ];
  if (next.is_undef()) {
    visit(succ);
    prev.update(next.low());
  } else if (next.on_stack()) {
    prev.update(next.index());
  }
}
Blocks Tarjan::collect(BB b) {
  Blocks comp;
  while (true) {
    const auto p = stack_.top();
    stack_.pop();
    frames_[p].pop();
    comp.push_back(p);
    if (b == p) {
      break;
    }
  }
  std::reverse(comp.begin(), comp.end());
  return comp;
}
Components Tarjan::run(const llvm::Function& f) {
  Tarjan tarjan{f};
  tarjan.comps_.init(f);
  return tarjan.comps_;
}

}  // namespace stacksafe
