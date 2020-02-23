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

Component::Component(const Blocks& vec) {
  for (const auto b : vec) {
    Super::emplace_back(*b);
  }
}
Component::Component(const std::vector<const llvm::BasicBlock*>& vec) {
  for (const auto b : vec) {
    Super::emplace_back(*b);
  }
}
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
  Super::push_back(b);
  state_.emplace_back();
}
void Components::init(const llvm::Function& f) {
  const auto b = &f.getEntryBlock();
  const auto pred = [b](const Blocks& c) { return c.contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  const auto index = std::distance(begin(), it);
  state_[index].init(f);
}
const Blocks& Components::find(BB b) const {
  const auto pred = [b](const Blocks& c) { return c.contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return *it;
}

Scc::Scc(const llvm::Function& f) : Super{Tarjan{f}.scc()} {
  back().map().init(f);
  current_ = rbegin();
}
Component& Scc::pop() {
  return *current_++;
}
bool Scc::empty() const {
  return current_ == rend();
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
const std::vector<Component>& Tarjan::scc() const {
  return scc_;
}
void Tarjan::visit(BB b) {
  stack_.push(b);
  auto& frame = frames_[b];
  frame.push(index_++);
  for (const auto& succ : Blocks::successors(b)) {
    update(frame, succ);
  }
  if (frame.is_root()) {
    const auto comp = collect(b);
    scc_.emplace_back(comp);
    comps_.push(comp);
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
