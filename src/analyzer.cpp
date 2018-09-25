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
    ClassNameVisitor classname;
    for (auto &bb : F.getBasicBlockList()) {
      for (auto &I : bb.getInstList()) {
        llvm::errs() << classname.visit(I) << "\n"
                     << I << "\n";
      }
    }
    return false;
  }
  Successors getSuccessors(llvm::BasicBlock &B) {
    Successors succ;
    if (auto t = B.getTerminator()) {
      const auto num = t->getNumSuccessors();
      succ.reserve(num);
      for (unsigned i = 0; i < num; ++i) {
        succ.push_back(t->getSuccessor(i));
      }
    }
    return succ;
  }

  void State::init(llvm::Function &F) {
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
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
