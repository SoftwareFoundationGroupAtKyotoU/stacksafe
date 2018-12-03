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
    ApplyVisitor apply(env, factory_);
    for (auto &I : B.getInstList()) {
      apply.visit(I);
    }
    return env;
  }

  ApplyVisitor::ApplyVisitor(Environment &env, LocationFactory &fac)
    : env_(env), fac_(fac)
  {}
  auto ApplyVisitor::visitAllocaInst(llvm::AllocaInst &I) -> RetTy {
    if (auto reg = make_register(I)) {
      auto loc = fac_.getLocal();
      env_.init(*reg).insert(loc);
      env_.init(loc);
    }
    visitInstruction(I);
  }
  auto ApplyVisitor::visitLoadInst(llvm::LoadInst &I) -> RetTy {
    if (auto reg = make_register(I)) {
      auto &target = env_.init(*reg);
      if (auto ptr = I.getPointerOperand()) {
        if (auto src = make_register(*ptr)) {
          if (env_.exists(*src)) {
            for (auto &loc : env_.get(*src)->get()) {
              if (env_.exists(loc)) {
                target.unify(env_.get(loc)->get());
              } else {
                llvm::errs() << spaces(make_manip(loc)) << "is not in heap" << endl;
              }
            }
          } else {
            llvm::errs() << spaces(make_manip(*src)) << "is not in stack" << endl;
          }
        }
      }
    }
    visitInstruction(I);
  }
  auto ApplyVisitor::visitInstruction(llvm::Instruction &I) -> RetTy {
    ClassNameVisitor classname;
    llvm::errs() << classname.visit(I) << endl;
    llvm::errs() << I << endl;
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
