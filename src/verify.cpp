#include "verify.hpp"

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
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy { return safe; }

}  // namespace stacksafe
