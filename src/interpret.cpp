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
    env_.constant(Value::create(i));
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
  } else {
    error(i);
  }
}
auto Interpret::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    env_.load(Value::create(i), Value::create(*src));
  } else {
    error(i);
  }
}
auto Interpret::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  if (lhs && rhs) {
    env_.binop(Value::create(i), Value::create(*lhs), Value::create(*rhs));
  } else {
    error(i);
  }
}
auto Interpret::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  env_.constant(Value::create(i));
}
auto Interpret::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (auto &a : i.args()) {
    if (auto v = a.get()) {
      params.push_back(Value::create(*v));
    } else {
      error(i);
    }
  }
  env_.call(Value::create(i), params);
}
auto Interpret::visitCastInst(llvm::CastInst &i) -> RetTy {
  if (auto v = i.getOperand(0)) {
    env_.cast(Value::create(i), Value::create(*v));
  } else {
    error(i);
  }
}
auto Interpret::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  if (auto v = i.getPointerOperand()) {
    env_.cast(Value::create(i), Value::create(*v));
  } else {
    error(i);
  }
}
auto Interpret::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (auto &use : i.incoming_values()) {
    if (auto val = use.get()) {
      params.push_back(Value::create(*val));
    } else {
      error(i);
    }
  }
  env_.phi(Value::create(i), params);
}
auto Interpret::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  if (auto v = i.getTrueValue()) {
    params.push_back(Value::create(*v));
  } else {
    error(i);
  }
  if (auto v = i.getFalseValue()) {
    params.push_back(Value::create(*v));
  } else {
    error(i);
  }
  env_.phi(Value::create(i), params);
}
auto Interpret::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getValOperand();
  if (ptr && val) {
    env_.cmpxchg(Value::create(i), Value::create(*ptr), Value::create(*val));
  } else {
    error(i);
  }
}
auto Interpret::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getNewValOperand();
  if (ptr && val) {
    env_.cmpxchg(Value::create(i), Value::create(*ptr), Value::create(*val));
  } else {
    error(i);
  }
}
void Interpret::error(llvm::Instruction &i) const {
  llvm::errs() << "Failed to process: " << i << "\n";
}

}  // namespace stacksafe
