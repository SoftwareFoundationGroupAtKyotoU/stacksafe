#include "verify.hpp"
#include "memory.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Verifier::Verifier(const Memory &e) : memory_{e} {}
auto Verifier::run(const llvm::BasicBlock *b, const Memory &memory) -> RetTy {
  auto dom = memory.from_heap(Symbol::global());
  if (dom.has_local()) {
    return unsafe;
  }
  return Verifier{memory}.visit(*b);
}
auto Verifier::visit(const llvm::BasicBlock &b) -> RetTy {
  for (auto &i : const_cast<llvm::BasicBlock &>(b)) {
    if (!Super::visit(i)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitCallInst(llvm::CallInst &i) -> RetTy {
  for (auto &use : i.args()) {
    if (auto arg = use.get()) {
      auto dom = memory_.from_stack(Value::make(*arg));
      if (dom.has_local() && dom.includes(Symbol::global())) {
        return unsafe;
      }
    } else {
      stacksafe_unreachable("unknown parameter", i);
    }
  }
  return safe;
}
auto Verifier::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto val = i.getReturnValue()) {
    auto dom = memory_.from_stack(Value::make(*val));
    if (dom.has_local()) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy { return safe; }

}  // namespace stacksafe
