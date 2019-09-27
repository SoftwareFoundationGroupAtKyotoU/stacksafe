#include "interpret.hpp"
#include "env.hpp"
#include "instruction.hpp"
#include "memory.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Interpreter::Interpreter(Memory &e) : memory_{e} {}
Memory Interpreter::run(const llvm::BasicBlock *b, const Env &pred) {
  auto ret = pred.memory();
  Interpreter{ret}.Super::visit(const_cast<llvm::BasicBlock &>(*b));
  return ret;
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  assert(i.isTerminator() && "unsupported instruction");
}
auto Interpreter::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  if (auto lhs = i.getOperand(0)) {
    if (auto rhs = i.getOperand(1)) {
      instr::binop(memory_, Value::make(i), Value::make(*lhs),
                   Value::make(*rhs));
    }
  }
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  instr::alloc(memory_, Value::make(i));
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    instr::load(memory_, Value::make(i), Value::make(*src));
  }
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  if (auto src = i.getValueOperand()) {
    if (auto dst = i.getPointerOperand()) {
      instr::store(memory_, Value::make(*src), Value::make(*dst));
    }
  }
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getNewValOperand()) {
      instr::cmpxchg(memory_, Value::make(i), Value::make(*ptr),
                     Value::make(*val));
    }
  }
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  if (auto ptr = i.getPointerOperand()) {
    if (auto val = i.getValOperand()) {
      instr::cmpxchg(memory_, Value::make(i), Value::make(*ptr),
                     Value::make(*val));
    }
  }
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  if (auto v = i.getOperand(0)) {
    instr::cast(memory_, Value::make(i), Value::make(*v));
  }
}
auto Interpreter::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  instr::constant(memory_, Value::make(i));
}
auto Interpreter::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (auto &use : i.incoming_values()) {
    if (auto val = use.get()) {
      params.push_back(Value::make(*val));
    } else {
      stacksafe_unreachable("unknown phi node", i);
    }
  }
  instr::phi(memory_, Value::make(i), params);
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
    instr::call(memory_, params);
  } else {
    instr::call(memory_, Value::make(i), params);
  }
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  if (auto v = i.getPointerOperand()) {
    instr::cast(memory_, Value::make(i), Value::make(*v));
  }
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
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
  instr::phi(memory_, Value::make(i), params);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  if (auto src = i.getAggregateOperand()) {
    instr::cast(memory_, Value::make(i), Value::make(*src));
  }
}

}  // namespace stacksafe
