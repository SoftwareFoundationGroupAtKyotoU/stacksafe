#include "interpret.hpp"
#include "debug.hpp"
#include "env.hpp"
#include "token.hpp"

namespace stacksafe {

Interpret::Interpret(Env &e) : env_{e} {}
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
auto Interpret::visit(llvm::Instruction &i) -> RetTy { return Base::visit(i); }
auto Interpret::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    unknown_instruction(i);
  }
}
auto Interpret::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  env_.alloc(Value::create(i));
}
auto Interpret::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    env_.store(Value::create(*src), Value::create(*dst));
  }
}
auto Interpret::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    env_.load(Value::create(i), Value::create(*src));
  }
}
auto Interpret::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  env_.constant(Value::create(i));
}
auto Interpret::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  env_.constant(Value::create(i));
}
auto Interpret::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (auto &a : i.args()) {
    if (auto v = a.get()) {
      params.push_back(Value::create(*v));
    }
  }
  env_.call(Value::create(i), params);
}
auto Interpret::visitCastInst(llvm::CastInst &i) -> RetTy {
  if (auto v = i.getOperand(0)) {
    env_.cast(Value::create(i), Value::create(*v));
  }
}
auto Interpret::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  if (auto v = i.getPointerOperand()) {
    env_.cast(Value::create(i), Value::create(*v));
  }
}
auto Interpret::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (auto &use : i.incoming_values()) {
    if (auto val = use.get()) {
      params.push_back(Value::create(*val));
    }
  }
  env_.phi(Value::create(i), params);
}
auto Interpret::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  if (auto v = i.getTrueValue()) {
    params.push_back(Value::create(*v));
  }
  if (auto v = i.getFalseValue()) {
    params.push_back(Value::create(*v));
  }
  env_.phi(Value::create(i), params);
}

}  // namespace stacksafe
