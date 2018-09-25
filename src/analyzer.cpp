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
    ClassNameVisitor classname;
    for (auto &bb : F.getBasicBlockList()) {
      for (auto &I : bb.getInstList()) {
        llvm::errs() << classname.visit(I) << "\n"
                     << I << "\n";
      }
    }
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
      if (auto t = b->getTerminator()) {
        auto next = update(*b);
        for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
          auto succ = t->getSuccessor(i);
          auto &prev = map_.at(succ);
          if (!next.subsetof(prev)) {
            prev.unify(next);
            todo_.push(succ);
          }
        }
      }
    }
  }
  Environment State::update(llvm::BasicBlock &B) {
    return map_.at(&B);
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
