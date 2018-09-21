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
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
