#include "interpret.hpp"
#include "env.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Interpreter::Interpreter(Env &e) : env_{e} {}
auto Interpreter::visit(const llvm::BasicBlock &b) -> RetTy {
  for (auto &i : const_cast<llvm::BasicBlock &>(b)) {
    Super::visit(i);
  }
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  assert(i.isTerminator() && "unsupported instruction");
}
auto Interpreter::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  env_.binop(i, *lhs, *rhs);
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  env_.alloc(i);
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  env_.load(i, *src);
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  assert(src && dst && "invalid operand");
  env_.store(*src, *dst);
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getNewValOperand();
  assert(ptr && val && "invalid operand");
  env_.cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getValOperand();
  assert(ptr && val && "invalid operand");
  env_.cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  auto src = i.getOperand(0);
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  instr::constant(env_, i);
}
auto Interpreter::visitPHINode(llvm::PHINode &i) -> RetTy {
  ValueSet params;
  for (auto &use : i.incoming_values()) {
    auto val = use.get();
    assert(val && "unknown phi node");
    params.insert(val);
  }
  env_.phi(i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  ValueSet params;
  for (auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.insert(arg);
  }
  if (check_voidfunc(i)) {
    instr::call(env_, params);
  } else {
    instr::call(env_, i, params);
  }
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  ValueSet params;
  for (auto val : {i.getTrueValue(), i.getFalseValue()}) {
    assert(val && "unknown select node");
    params.insert(val);
  }
  env_.phi(i, params);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  env_.cast(i, *src);
}

}  // namespace stacksafe
