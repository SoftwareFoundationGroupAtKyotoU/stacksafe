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

Block::Block(const llvm::BasicBlock& b) : block_{&b} {}
std::vector<Block> Block::successors() const {
  std::vector<Block> vec;
  const auto t = block_->getTerminator();
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    vec.emplace_back(*t->getSuccessor(i));
  }
  return vec;
}
const llvm::BasicBlock& Block::get() const {
  return *block_;
}
bool Block::operator==(const Block& block) const {
  return block_ == block.block_;
}

Component::Component(const std::vector<Block>& vec) : Super{vec} {}
bool Component::contains(const Block& b) const {
  return std::find(begin(), end(), b) != end();
}
bool Component::is_loop() const {
  for (const auto& b : *this) {
    for (const auto& succ : b.successors()) {
      if (contains(succ)) {
        return true;
      }
    }
  }
  return false;
}
std::vector<Block> Component::successors() const {
  std::vector<Block> out;
  for (const auto& b : *this) {
    for (const auto& succ : b.successors()) {
      if (!contains(succ) &&
          std::find(out.begin(), out.end(), succ) == out.end()) {
        out.Super::emplace_back(succ);
      }
    }
  }
  return out;
}
void Component::merge(const Component& c) {
  map_.merge(c.map_);
}
Map& Component::map() {
  return map_;
}

Scc::Scc(const llvm::Function& f) : Super{Tarjan{f}.scc()} {
  back().map().init(f);
}
Component Scc::pop() {
  auto ret = std::move(back());
  pop_back();
  return ret;
}
Component& Scc::find(const Block& b) {
  const auto pred = [&b](const Component& c) { return c.contains(b); };
  auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "SCC must be a partition");
  return *it;
}
void Scc::distribute(const Component& prev) {
  for (const auto& succ : prev.successors()) {
    auto& next = find(succ);
    next.merge(prev);
  }
}

Tarjan::Tarjan(const llvm::Function& f) : frames_{f.size()}, index_{0} {
  std::size_t i = 0;
  for (const auto& b : f) {
    map_.try_emplace(&b, &frames_[i]);
    ++i;
  }
  for (const auto& b : f) {
    const Block block{b};
    if (map(block).is_undef()) {
      visit(block);
    }
  }
}
const std::vector<Component>& Tarjan::scc() const {
  return scc_;
}
void Tarjan::visit(const Block& b) {
  Frame& frame = push(b);
  for (const auto& succ : b.successors()) {
    update(frame, succ);
  }
  if (frame.is_root()) {
    scc_.emplace_back(collect(b));
  }
}
Frame& Tarjan::push(const Block& b) {
  Frame& frame = map(b);
  frame.push(index_);
  ++index_;
  stack_.push(b);
  return frame;
}
void Tarjan::update(Frame& frame, const Block& succ) {
  const Frame& f = map(succ);
  if (f.is_undef()) {
    visit(succ);
    frame.update(f.low());
  } else if (f.on_stack()) {
    frame.update(f.index());
  }
}
Block Tarjan::pop() {
  const auto b = stack_.top();
  stack_.pop();
  map(b).pop();
  return b;
}
Component Tarjan::collect(const Block& b) {
  std::vector<Block> comp;
  while (true) {
    const auto p = pop();
    comp.emplace_back(p);
    if (b == p) {
      break;
    }
  }
  std::reverse(comp.begin(), comp.end());
  return Component{comp};
}
Frame& Tarjan::map(const Block& b) {
  return *map_[&b.get()];
}

}  // namespace stacksafe
