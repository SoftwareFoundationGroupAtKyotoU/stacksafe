#include "analyzer.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  char Analyzer::ID = 0;
  Analyzer::Analyzer()
    : llvm::FunctionPass(ID)
  {}
  bool Analyzer::runOnFunction(llvm::Function &F) {
    for (const auto &bb : F.getBasicBlockList()) {
      llvm::errs() << bb << "\n";
    }
    return false;
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "Analyzer Pass for Stack Temporal Memory Safety");
