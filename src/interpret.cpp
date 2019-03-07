#include "interpret.hpp"

namespace stacksafe {
  Interpret::Interpret(Env &env)
    : env_(env) {
  }
  void Interpret::visit(llvm::BasicBlock &B) {
    Base::visit(B);
  }
  bool Interpret::visit(llvm::Instruction &I) {
    llvm::outs() << I << endl;
    if (I.isTerminator()) {
      llvm::outs() << env_;
    } else if (!Base::visit(I)) {
      llvm::errs() << "Error: something wrong happens" << endl;
      return false;
    }
    return true;
  }
  bool Interpret::visitInstruction(llvm::Instruction &I) {
    ClassNameVisitor name;
    llvm::outs() << name.visit(I) << endl;
    return false;
  }
  bool Interpret::visitAllocaInst(llvm::AllocaInst &I) {
    if (auto reg = make_register(I)) {
      return env_.alloca(*reg);
    }
    return error(__func__);
  }
  bool Interpret::visitStoreInst(llvm::StoreInst &I) {
    if (auto ptr = I.getPointerOperand()) {
      if (auto dst = make_register(*ptr)) {
        if (auto val = I.getValueOperand()) {
          return env_.store(Value{*val}, *dst);
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitLoadInst(llvm::LoadInst &I) {
    if (auto ptr = I.getPointerOperand()) {
      if (auto src = make_register(*ptr)) {
        if (auto dst = make_register(I)) {
          return env_.load(*dst, *src);
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitGetElementPtrInst(llvm::GetElementPtrInst &I) {
    if (auto dst = make_register(I)) {
      if (auto ptr = I.getPointerOperand()) {
        if (auto src = make_register(*ptr)) {
          return env_.getelementptr(*dst, *src);
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitBinaryOperator(llvm::BinaryOperator &I) {
    if (auto dst = make_register(I)) {
      return env_.binary(*dst);
    }
    return error(__func__);
  }
  bool Interpret::visitCastInst(llvm::CastInst &I) {
    if (auto dst = make_register(I)) {
      if (auto val = I.getOperand(0)) {
        return env_.cast(*dst, Value{*val});
      }
    }
    return error(__func__);
  }
  bool Interpret::visitPHINode(llvm::PHINode &I) {
    if (auto dst = make_register(I)) {
      for (auto &use: I.incoming_values()) {
        return env_.phi(*dst, Value{*use.get()});
      }
    }
    return error(__func__);
  }
  bool Interpret::visitCmpInst(llvm::CmpInst &I) {
    if (auto dst = make_register(I)) {
      return env_.cmp(*dst);
    }
    return error(__func__);
  }
  bool Interpret::visitCallInst(llvm::CallInst &I) {
    if (env_.call(I.arg_operands(), make_register(I))) {
      return true;
    }
    return error(__func__);
  }
  bool Interpret::error(const char *name) {
    llvm::errs() << endl << "Error: unknown error in " << name << endl;
    return false;
  }

  Env interpret(llvm::BasicBlock &b, Env env) {
    Interpret{env}.visit(b);
    return env;
  }
}
