#include "verify.hpp"
#include "env.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
auto Verifier::visit(const llvm::BasicBlock &b) -> RetTy {
  if (env_.from_heap(Symbol::global()).has_local()) {
    return unsafe;
  }
  for (auto &i : const_cast<llvm::BasicBlock &>(b)) {
    if (!Super::visit(i)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitCallInst(llvm::CallInst &i) -> RetTy {
  for (auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    auto dom = env_.lookup(*arg);
    if (dom.has_local() && dom.includes(Symbol::global())) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto val = i.getReturnValue()) {
    auto dom = env_.lookup(*val);
    if (dom.has_local()) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy {
  return safe;
}

}  // namespace stacksafe
