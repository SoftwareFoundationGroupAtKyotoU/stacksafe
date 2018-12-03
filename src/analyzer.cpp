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
        if (!entry.initArg(*reg)) {
          llvm::errs() << "Error: Something wrong happens" << endl;
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
  auto ApplyVisitor::visitAllocaInst(llvm::AllocaInst &I) -> RetTy {
    visitInstruction(I);
    if (auto reg = make_register(I)) {
      return env_.alloca(*reg);
    }
    llvm::errs() << "Error: Unknown error in visitAllocaInst" << endl;
    return false;
  }
  auto ApplyVisitor::visitLoadInst(llvm::LoadInst &I) -> RetTy {
    visitInstruction(I);
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
    visitInstruction(I);
    if (auto val = I.getValueOperand()) {
      if (auto src = make_register(*val)) {
        if (auto ptr = I.getPointerOperand()) {
          if (auto dst = make_register(*ptr)) {
            return env_.store(*src, *dst);
          }
        }
      }
    }
    llvm::errs() << "Error: Unknown error in visitStoreInst" << endl;
    return false;
  }
  auto ApplyVisitor::visitInstruction(llvm::Instruction &I) -> RetTy {
    llvm::errs() << env_;
    ClassNameVisitor classname;
    llvm::errs() << classname.visit(I) << endl;
    llvm::errs() << I << endl;
    return true;
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
