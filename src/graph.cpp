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
std::vector<Block> Component::out_degree() const {
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
void Component::merge(const Map& m) {
  map_.merge(m);
}

SCC::SCC(const llvm::Function& f) {
  Tarjan tarjan{f};
  auto scc = tarjan.scc();
  for (const auto& c : scc) {
    std::vector<Block> bs;
    for (const auto& b : c) {
      bs.emplace_back(*b);
    }
    Super::emplace_back(bs);
  }
  back().merge(Map{f});
}
Component SCC::pop() {
  auto ret = std::move(back());
  pop_back();
  return ret;
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
const std::vector<Scc>& Tarjan::scc() const {
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
Scc Tarjan::collect(const Block& b) {
  Scc scc;
  while (true) {
    const auto p = pop();
    scc.emplace_back(&p.get());
    if (b == p) {
      break;
    }
  }
  std::reverse(scc.begin(), scc.end());
  return scc;
}
Frame& Tarjan::map(const Block& b) {
  return *map_[&b.get()];
}

bool Scc::contains(const llvm::BasicBlock* b) const {
  return std::find(begin(), end(), b) != end();
}
bool Scc::is_loop() const {
  for (const auto& b : *this) {
    const auto t = b->getTerminator();
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      const auto succ = t->getSuccessor(i);
      if (contains(succ)) {
        return true;
      }
    }
  }
  return false;
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
void Scc::merge(const Map& map) {
  map_.merge(map);
}
void Scc::convey() {
  for (const auto ptr : succ_) {
    ptr->merge(map_);
  }
}
Map& Scc::map() {
  return map_;
}
auto Scc::decompose(const llvm::Function& f) -> Stack {
  Tarjan tarjan{f};
  std::vector<SccPtr> ret;
  for (auto scc : tarjan.scc()) {
    for (const auto& b : scc.out_degree()) {
      const auto pred = [b](const SccPtr& ptr) { return ptr->contains(b); };
      if (auto it = std::find_if(ret.begin(), ret.end(), pred);
          it != ret.end()) {
        scc.add_successor(*it);
        assert(std::find_if(std::next(it), ret.end(), pred) == ret.end() &&
               "multiple successor SCCs");
      } else {
        llvm_unreachable("ERROR: broken SCC");
      }
    }
    ret.emplace_back(std::make_shared<Scc>(std::move(scc)));
  }
  return Scc::Stack{ret};
}

}  // namespace stacksafe
