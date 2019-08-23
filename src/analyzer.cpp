#include "analyzer.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "visualize.hpp"

namespace stacksafe {
char Analyzer::ID = 0;
Analyzer::Analyzer() : llvm::FunctionPass(ID) {}
void Analyzer::print(llvm::raw_ostream &O, const llvm::Module *M) const {
  llvm::outs() << "StackSafe" << endl;
}
bool Analyzer::runOnFunction(llvm::Function &F) {
  abst_ = std::make_unique<Abstraction>(F);
  abst_->proceed();
  return false;
}
}  // namespace stacksafe

static llvm::RegisterPass<stacksafe::Analyzer> registerpass(
    "stacksafe", "Analyzer pass for temporal memory safety of the stack");
