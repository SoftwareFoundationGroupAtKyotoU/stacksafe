#include "verify.hpp"
#include "env.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
auto Verifier::visit(const llvm::BasicBlock &b) -> RetTy {
  if (env_.lookup(Symbol::global()).has_local()) {
    return unsafe;
  }
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    if (!Super::visit(i)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitCallInst(llvm::CallInst &i) -> RetTy {
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    if (env_.has_local(*arg) && env_.has_global(*arg)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto ret = i.getReturnValue()) {
    if (env_.has_local(*ret)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy {
  return safe;
}

}  // namespace stacksafe
