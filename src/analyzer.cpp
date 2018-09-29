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
    static const Environment empty;
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
    ClassNameVisitor classname;
    for (auto &I : B.getInstList()) {
      llvm::errs() << classname.visit(I) << endl
                   << I << endl;
    }
    return map_.at(&B);
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
