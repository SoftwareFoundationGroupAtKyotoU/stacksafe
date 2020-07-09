#include "state.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include "tarjan.hpp"
#include "utility.hpp"

namespace stacksafe {

State::State(const llvm::Function &f) {
  for (const auto &c : tarjan::BlockSolver::scc(f)) {
    Super::emplace_back(c);
  }
  find(&f.getEntryBlock()).init(f);
}
void State::transfer(const Component &c) {
  for (const auto &succ : c.blocks().successors()) {
    find(succ).merge(c);
  }
}
bool State::is_safe() const {
  const auto pred = [](const Component &c) { return c.is_safe(); };
  return std::all_of(begin(), end(), pred);
}
Component &State::find(BB b) {
  const auto pred = [b](const Component &c) { return c.blocks().contains(b); };
  const auto it = std::find_if(begin(), end(), pred);
  assert(it != end() && std::find_if(std::next(it), end(), pred) == end() &&
         "components must be a partition");
  return *it;
}

}  // namespace stacksafe

namespace dataflow {
void ValueSet::insert(const ValueSet &set) {
  for (const auto &v : set) {
    Super::insert(v);
  }
}
ValueSet State::eval(const llvm::Value *v) const {
  ValueSet ret;
  if (llvm::isa<llvm::Argument>(v)) {
    ret.insert(v);
  } else if (auto i = llvm::dyn_cast<llvm::BinaryOperator>(v)) {
    ret.insert(eval(i->getOperand(0)));
    ret.insert(eval(i->getOperand(1)));
  } else if (llvm::isa<llvm::AllocaInst>(v)) {
    ret.insert(v);
  } else if (auto i = llvm::dyn_cast<llvm::LoadInst>(v)) {
    auto p = i->getPointerOperand();
    for (const auto &v : eval(p)) {
      if (auto pair = Super::find(v); pair != Super::end()) {
        ret.insert(pair->second);
      } else {
        debug::print("invalid load: " + debug::to_str(*v));
      }
    }
  } else if (auto i = llvm::dyn_cast<llvm::GetElementPtrInst>(v)) {
    ret.insert(eval(i->getPointerOperand()));
  } else if (auto i = llvm::dyn_cast<llvm::CastInst>(v)) {
    ret.insert(eval(i->getOperand(0)));
  } else if (auto i = llvm::dyn_cast<llvm::PHINode>(v)) {
    for (const auto &use : i->incoming_values()) {
      ret.insert(eval(use.get()));
    }
  } else if (auto i = llvm::dyn_cast<llvm::SelectInst>(v)) {
    ret.insert(eval(i->getTrueValue()));
    ret.insert(eval(i->getFalseValue()));
  } else if (v) {
    debug::print("unsupported eval: " + debug::to_str(*v));
  }
  return ret;
}
}  // namespace dataflow
