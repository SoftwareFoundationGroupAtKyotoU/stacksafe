#include "interpret.hpp"
#include "env.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Interpret::Interpret(Env &e) : env_{e} {}
Env Interpret::run(const llvm::BasicBlock *b, const Env &pred) {
  auto ret = pred;
  Interpret{ret}.visit(const_cast<llvm::BasicBlock &>(*b));
  return ret;
}
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
auto Interpret::visit(llvm::Instruction &i) -> RetTy { return Base::visit(i); }
auto Interpret::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    stacksafe_unreachable("unsupported instruction", i);
  }
}
auto Interpret::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  if (auto lhs = i.getOperand(0)) {
    if (auto rhs = i.getOperand(1)) {
      env_.binop(Value::make(i), Value::make(*lhs), Value::make(*rhs));
    }
  }
}
auto Interpret::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  env_.alloc(Value::make(i));
}
auto Interpret::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    env_.load(Value::make(i), Value::make(*src));
  }
}
auto Interpret::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  if (auto src = i.getValueOperand()) {
    if (auto dst = i.getPointerOperand()) {
      env_.store(Value::make(*src), Value::make(*dst));
    }
  }
}
auto Interpret::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getNewValOperand()) {
      env_.cmpxchg(Value::make(i), Value::make(*ptr), Value::make(*val));
    }
  }
}
auto Interpret::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getValOperand()) {
      env_.cmpxchg(Value::make(i), Value::make(*ptr), Value::make(*val));
    }
  }
}
auto Interpret::visitCastInst(llvm::CastInst &i) -> RetTy {
  if (auto v = i.getOperand(0)) {
    env_.cast(Value::make(i), Value::make(*v));
  }
}
auto Interpret::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  env_.constant(Value::make(i));
}
auto Interpret::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (auto &use : i.incoming_values()) {
    if (auto val = use.get()) {
      params.push_back(Value::make(*val));
    } else {
      stacksafe_unreachable("unknown phi node", i);
    }
  }
  env_.phi(Value::make(i), params);
}
auto Interpret::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (auto &a : i.args()) {
    if (auto v = a.get()) {
      params.push_back(Value::make(*v));
    } else {
      stacksafe_unreachable("unknown parameter", i);
    }
  }
  if (is_voidfunc(i)) {
    env_.call(params);
  } else {
    env_.call(Value::make(i), params);
  }
}
auto Interpret::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  if (auto v = i.getPointerOperand()) {
    env_.cast(Value::make(i), Value::make(*v));
  }
}
auto Interpret::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  if (auto v = i.getTrueValue()) {
    params.push_back(Value::make(*v));
  } else {
    stacksafe_unreachable("unknown select node", i);
  }
  if (auto v = i.getFalseValue()) {
    params.push_back(Value::make(*v));
  } else {
    stacksafe_unreachable("unknown select node", i);
  }
  env_.phi(Value::make(i), params);
}
auto Interpret::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  if (auto src = i.getAggregateOperand()) {
    env_.cast(Value::make(i), Value::make(*src));
  }
}

}  // namespace stacksafe
