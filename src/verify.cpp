#include "verify.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
void Verifier::run(const llvm::BasicBlock *b, const Env &pred) {
  Verifier{pred}.Super::visit(const_cast<llvm::BasicBlock &>(*b));
}

}  // namespace stacksafe
