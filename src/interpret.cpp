#include "interpret.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Interpreter::Interpreter(const Memory &e) : env_{e}, memory_{env_.memory()} {}
Memory Interpreter::run(const llvm::BasicBlock *b, const Env &pred) {
  Interpreter i{pred.memory()};
  i.Super::visit(const_cast<llvm::BasicBlock &>(*b));
  return i.memory_;
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  assert(i.isTerminator() && "unsupported instruction");
}
auto Interpreter::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  if (auto lhs = i.getOperand(0)) {
    if (auto rhs = i.getOperand(1)) {
      instr::binop(env_, i, *lhs, *rhs);
    }
  }
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  instr::alloc(env_, i);
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    instr::load(env_, i, *src);
  }
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  if (auto src = i.getValueOperand()) {
    if (auto dst = i.getPointerOperand()) {
      instr::store(env_, *src, *dst);
    }
  }
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getNewValOperand()) {
      instr::cmpxchg(env_, i, *ptr, *val);
    }
  }
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getValOperand()) {
      instr::cmpxchg(env_, i, *ptr, *val);
    }
  }
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  if (auto src = i.getOperand(0)) {
    instr::cast(env_, i, *src);
  }
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
  instr::phi(env_, i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (auto &use : i.args()) {
    if (auto arg = use.get()) {
      params.push_back(Value::make(*arg));
    } else {
      stacksafe_unreachable("unknown parameter", i);
    }
  }
  if (check_voidfunc(i)) {
    instr::call(env_, params);
  } else {
    instr::call(env_, i, params);
  }
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    instr::cast(env_, i, *src);
  }
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  ValueSet params;
  for (auto val : {i.getTrueValue(), i.getFalseValue()}) {
    assert(val && "unknown select node");
    params.insert(val);
  }
  instr::phi(env_, i, params);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  if (auto src = i.getAggregateOperand()) {
    instr::cast(env_, i, *src);
  }
}

}  // namespace stacksafe
