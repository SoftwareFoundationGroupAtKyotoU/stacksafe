#include "tarjan.hpp"
#include <llvm/IR/Function.h>

namespace stacksafe {

Components Tarjan::run(const llvm::Function& f) {
  Components comps;
  Tarjan tarjan{f};
  for (const auto& b : f) {
    tarjan.visit(&b, comps);
  }
  for (auto& [c, m] : comps) {
    std::reverse(c.begin(), c.end());
  }
  std::reverse(comps.begin(), comps.end());
  comps.find(&f.getEntryBlock()).init(f);
  return comps;
}
Tarjan::Tarjan(const llvm::Function& f) : index_{0} {
  for (const auto& b : f) {
    frames_.try_emplace(&b);
  }
}
bool Tarjan::visit(BB b, Components& comps) {
  const auto ok = frames_[b].is_undef();
  if (ok) {
    auto& frame = push(b);
    for (const auto& succ : Blocks::successors(b)) {
      const auto& next = frames_[succ];
      if (visit(succ, comps)) {
        frame.update(next.low());
      } else if (next.on_stack()) {
        frame.update(next.index());
      }
    }
    if (frame.is_root()) {
      Blocks comp;
      while (true) {
        const auto p = pop();
        comp.push_back(p);
        if (b == p) {
          break;
        }
      }
      comps.emplace_back(comp, Map{});
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
