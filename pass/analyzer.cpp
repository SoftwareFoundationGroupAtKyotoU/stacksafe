#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <memory>
#include "abstract.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  std::unique_ptr<Abstract> abst;
  Analyzer() : llvm::FunctionPass{ID} {}
  bool runOnFunction(llvm::Function &f) override {
    llvm::outs() << "Analysis 'stacksafe' for function '" << f.getName()
                 << "'\n";
    llvm::outs().flush();
    abst = std::make_unique<Abstract>(f);
    assert(abst && "allocation failed");
    abst->run_scc(f);
    return false;
  }
  void print(llvm::raw_ostream &os, const llvm::Module *) const override {
    assert(abst && "allocation failed");
    abst->print(os);
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe", "stacksafe",
                                                 false, true);
}  // namespace stacksafe
