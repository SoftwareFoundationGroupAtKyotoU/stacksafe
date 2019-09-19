#include "verify.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
auto Verifier::run(const llvm::BasicBlock *b, const Env &pred) -> RetTy {
  Verifier v{pred};
  for (auto &i : const_cast<llvm::BasicBlock &>(*b)) {
    if (!v.visit(i)) {
      return false;
    }
  }
  return true;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy { return true; }

}  // namespace stacksafe
