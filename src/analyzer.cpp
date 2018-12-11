#include "analyzer.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  char Analyzer::ID = 0;
  Analyzer::Analyzer()
    : llvm::FunctionPass(ID)
  {}
  bool Analyzer::runOnFunction(llvm::Function &F) {
    state_ = std::make_unique<State>(F);
    state_->traverse();
    return false;
  }

  State::State(llvm::Function &F) {
    todo_.push(&F.getEntryBlock());
    static const Environment empty(factory_);
    for (auto &b : F.getBasicBlockList()) {
      map_.emplace(&b, empty);
    }
    auto &entry = map_.at(todo_.front());
    for (auto &a: F.args()) {
      if (auto reg = make_register(a)) {
        if (!entry.argument(*reg)) {
          llvm::errs() << "Error:" << spaces(make_manip(*reg))
                       << "is not an argument" << endl;
        }
      }
    }
  }
  void State::traverse() {
    while (!todo_.empty()) {
      auto b = todo_.front();
      todo_.pop();
      auto prev = update(*b);
      if (auto t = b->getTerminator()) {
        propagate(prev, *t);
      }
    }
  }
  void State::propagate(const Environment &prev, llvm::TerminatorInst &I) {
    for (unsigned i = 0; i < I.getNumSuccessors(); ++i) {
      auto succ = I.getSuccessor(i);
      auto &next = map_.at(succ);
      if (!prev.subsetof(next)) {
        next.unify(prev);
        todo_.push(succ);
      }
    }
  }
  Environment State::update(llvm::BasicBlock &B) {
    auto &env = map_.at(&B);
    ApplyVisitor apply(env);
    for (auto &I : B.getInstList()) {
      if (!apply.visit(I)) {
        llvm::errs() << "Error: Something wrong happens" << endl;
      }
    }
    return env;
  }

  ApplyVisitor::ApplyVisitor(Environment &env)
    : env_(env)
  {}
  auto ApplyVisitor::visit(llvm::Instruction &I) -> RetTy {
    auto ret = Base::visit(I);
    llvm::errs() << I << endl;
    return ret;
  }
  auto ApplyVisitor::visitAllocaInst(llvm::AllocaInst &I) -> RetTy {
    if (auto reg = make_register(I)) {
      return env_.alloca(*reg);
    }
    llvm::errs() << "Error: Unknown error in visitAllocaInst" << endl;
    return false;
  }
  auto ApplyVisitor::visitLoadInst(llvm::LoadInst &I) -> RetTy {
    if (auto ptr = I.getPointerOperand()) {
      if (auto src = make_register(*ptr)) {
        if (auto dst = make_register(I)) {
          return env_.load(*dst, *src);
        }
      }
    }
    llvm::errs() << "Error: Unknown error in visitLoadInst" << endl;
    return false;
  }
  auto ApplyVisitor::visitStoreInst(llvm::StoreInst &I) -> RetTy {
    if (auto ptr = I.getPointerOperand()) {
      if (auto dst = make_register(*ptr)) {
        if (auto val = I.getValueOperand()) {
          Value src{*val};
          return env_.store(src, *dst);
        }
      }
    }
    llvm::errs() << "Error: Unknown error in visitStoreInst" << endl;
    return false;
  }
  auto ApplyVisitor::visitGetElementPtrInst(llvm::GetElementPtrInst &I)
    -> RetTy {
    if (auto dst = make_register(I)) {
      if (auto ptr = I.getPointerOperand()) {
        if (auto src = make_register(*ptr)) {
          return env_.getelementptr(*dst, *src);
        }
      }
    }
    return false;
  }
  auto ApplyVisitor::visitBinaryOperator(llvm::BinaryOperator &I) -> RetTy {
    if (auto dst = make_register(I)) {
      return env_.binary(*dst);
    }
    return false;
  }
  auto ApplyVisitor::visitCastInst(llvm::CastInst &I) -> RetTy {
    if (auto dst = make_register(I)) {
      if (auto val = I.getOperand(0)) {
        if (llvm::isa<llvm::Constant>(val)) {
          return env_.binary(*dst);
        } else if (auto src = make_register(*val)) {
          return env_.cast(*dst, *src);
        }
      }
    }
    return false;
  }
  auto ApplyVisitor::visitPHINode(llvm::PHINode &I) -> RetTy {
    if (auto dst = make_register(I)) {
      for (auto &use: I.incoming_values()) {
        auto val = use.get();
        if (llvm::isa<llvm::Constant>(val)) {
          if (!env_.binary(*dst)) {
            return false;
          }
        } else if (auto src = make_register(*val)) {
          if (!env_.phi(*dst, *src)) {
            return false;
          }
        }
      }
    }
    return false;
  }
  auto ApplyVisitor::visitCmpInst(llvm::CmpInst &I) -> RetTy {
    if (auto dst = make_register(I)) {
      return env_.binary(*dst);
    }
    return false;
  }
  auto ApplyVisitor::visitInstruction(llvm::Instruction &I) -> RetTy {
    ClassNameVisitor classname;
    llvm::errs() << classname.visit(I) << endl;
    return true;
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
