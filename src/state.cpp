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
Value State::eval(const llvm::Value *v) const {
  return Value{v};
}
Value State::eval(const Cell &cell) const {
  return eval(cell.value());
}
void State::update(const Cell &key, const Value &val) {
  auto [it, _] = Super::try_emplace(key);
  it->second = val;
}
void State::transfer(const llvm::BasicBlock &b) {
  for (const auto &i : b) {
    if (auto store = llvm::dyn_cast<llvm::StoreInst>(&i)) {
      auto src = eval(store->getValueOperand());
      auto dst = eval(store->getPointerOperand());
      for (const auto &key : dst) {
        update(key, src);
      }
    }
  }
}
void to_json(nlohmann::json &j, const State &state) {
  std::map<std::string, Value> base;
  for (const auto &[key, val] : state) {
    base.try_emplace(key.to_string(), val);
  }
  j = base;
}
}  // namespace dataflow
