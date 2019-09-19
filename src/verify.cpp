#include "verify.hpp"
#include "env.hpp"
#include "value.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
auto Verifier::run(const llvm::BasicBlock *b, const Env &pred) -> RetTy {
  return Verifier{pred}.visit(*b);
}
auto Verifier::visit(const llvm::BasicBlock &b) -> RetTy {
  for (auto &i : const_cast<llvm::BasicBlock &>(b)) {
    if (!Super::visit(i)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto r = i.getReturnValue()) {
    auto v = Value::make(*r);
    if (v.is_register()) {
      if (auto dom = env_.stack().get(v)) {
        if (dom->has_local()) {
          return unsafe;
        }
      }
    }
  }
  return safe;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy { return safe; }

}  // namespace stacksafe
