#include "interpret.hpp"
#include "environment.hpp"

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
      debugs() << env_;
    } else if (!Base::visit(I)) {
      debugs() << "Error: something wrong happens" << endl;
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
      if (env_.alloca(*reg)) {
        return true;
      }
    }
    return error(__func__);
  }
  bool Interpret::visitStoreInst(llvm::StoreInst &I) {
    if (auto ptr = I.getPointerOperand()) {
      if (auto dst = make_register(*ptr)) {
        if (auto val = I.getValueOperand()) {
          if (env_.store(Value{*val}, *dst)) {
            return true;
          }
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitLoadInst(llvm::LoadInst &I) {
    if (auto ptr = I.getPointerOperand()) {
      if (auto src = make_register(*ptr)) {
        if (auto dst = make_register(I)) {
          if (env_.load(*dst, *src)) {
            return true;
          }
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitGetElementPtrInst(llvm::GetElementPtrInst &I) {
    if (auto dst = make_register(I)) {
      if (auto ptr = I.getPointerOperand()) {
        if (auto src = make_register(*ptr)) {
          if (env_.getelementptr(*dst, *src)) {
            return true;
          }
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitBinaryOperator(llvm::BinaryOperator &I) {
    if (auto dst = make_register(I)) {
      if (env_.binary(*dst)) {
        return true;
      }
    }
    return error(__func__);
  }
  bool Interpret::visitCastInst(llvm::CastInst &I) {
    if (auto dst = make_register(I)) {
      if (auto val = I.getOperand(0)) {
        if (env_.cast(*dst, Value{*val})) {
          return true;
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitPHINode(llvm::PHINode &I) {
    if (auto dst = make_register(I)) {
      for (auto &use: I.incoming_values()) {
        if (env_.phi(*dst, Value{*use.get()})) {
          return true;
        }
      }
    }
    return error(__func__);
  }
  bool Interpret::visitCmpInst(llvm::CmpInst &I) {
    if (auto dst = make_register(I)) {
      if (env_.cmp(*dst)) {
        return true;
      }
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
    debugs() << endl << "Error: unknown error in " << name << endl;
    return false;
  }

  Env interpret(llvm::BasicBlock &b, Env env) {
    Interpret{env}.visit(b);
    return env;
  }
}
