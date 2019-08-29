#include "interpret.hpp"
#include <llvm/Support/raw_ostream.h>
#include "register.hpp"

namespace stacksafe {

void Interpret::visit(llvm::Function &f) {
  for (auto &a : f.args()) {
    abst_.alloc(Register{a});
  }
  llvm::errs() << f;
  Base::visit(f);
}
void Interpret::visit(llvm::BasicBlock &b) {
  Base::visit(b);
  abst_.show();
}
void Interpret::visit(llvm::Instruction &i) { Base::visit(i); }
void Interpret::visitAllocaInst(llvm::AllocaInst &i) {
  abst_.alloc(Register{i});
}
void Interpret::visitStoreInst(llvm::StoreInst &i) {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    abst_.store(Register{*src}, Register{*dst});
  }
}
void Interpret::visitLoadInst(llvm::LoadInst &i) {
  if (auto src = i.getPointerOperand()) {
    abst_.load(Register{i}, Register{*src});
  }
}

}  // namespace stacksafe
