#include "transfer.hpp"
#include "state.hpp"
#include "utility.hpp"

namespace dataflow {
Transfer::Transfer(State &state) : state_{state} {}
auto Transfer::visitInstruction(llvm::Instruction &i) -> RetTy {
  debug::print("unsupported instruction: " + debug::to_str(i));
}
void Transfer::apply(const llvm::BasicBlock &b) const {
  for (const auto &i : b) {
    if (auto store = llvm::dyn_cast<llvm::StoreInst>(&i)) {
      auto src = state_.eval(store->getValueOperand());
      auto dst = state_.eval(store->getPointerOperand());
      for (const auto &key : dst) {
        state_.update(key, src);
      }
    }
  }
}
}  // namespace dataflow
