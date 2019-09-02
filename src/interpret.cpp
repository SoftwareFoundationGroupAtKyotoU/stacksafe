#include "interpret.hpp"
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"
#include "value.hpp"

namespace stacksafe {

Interpret::Interpret(const Env &e) : abst_{e} {}
const Env &Interpret::get() const { return abst_; }
void Interpret::visit(llvm::Function &f) {
  for (auto &a : f.args()) {
    abst_.argument(Value{a});
  }
  llvm::errs() << f;
  Base::visit(f);
}
void Interpret::visit(llvm::BasicBlock &b) {
  Base::visit(b);
  Json j = abst_;
  llvm::errs() << j.dump(2) << "\n";
}
void Interpret::visit(llvm::Instruction &i) { Base::visit(i); }
void Interpret::visitAllocaInst(llvm::AllocaInst &i) { abst_.alloc(Value{i}); }
void Interpret::visitStoreInst(llvm::StoreInst &i) {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    abst_.store(Value{*src}, Value{*dst});
  }
}
void Interpret::visitLoadInst(llvm::LoadInst &i) {
  if (auto src = i.getPointerOperand()) {
    abst_.load(Value{i}, Value{*src});
  }
}
void Interpret::visitBinaryOperator(llvm::BinaryOperator &i) {
  abst_.binop(Value{i});
}

}  // namespace stacksafe
