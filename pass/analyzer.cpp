#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/Format.h>
#include <memory>
#include "abstract.hpp"

namespace stacksafe {
namespace {
void endline(llvm::raw_ostream &os) {
  (os << "\n").flush();
}
constexpr auto stacksafe{"stacksafe"};
}  // namespace

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  std::unique_ptr<Abstract> abst;
  Analyzer();
  bool runOnFunction(llvm::Function &f) override;
  void print(llvm::raw_ostream &os, const llvm::Module *m) const override;
};

char Analyzer::ID = 0;
Analyzer::Analyzer() : llvm::FunctionPass{ID} {}
bool Analyzer::runOnFunction(llvm::Function &f) {
  const auto name = f.getName().str();
  const auto msg =
      llvm::format("Analysis '%s' for function '%s'", stacksafe, name.c_str());
  endline(llvm::outs() << msg);
  abst = std::make_unique<Abstract>(f);
  if (abst) {
    abst->interpret();
  } else {
    endline(llvm::errs() << "ERROR: allocation failed");
  }
  return false;
}
void Analyzer::print(llvm::raw_ostream &os, const llvm::Module *) const {
  if (abst) {
    abst->print(os);
    abst->print_depend(os);
  }
}

static llvm::RegisterPass<Analyzer> registerpass(stacksafe, stacksafe, false,
                                                 true);
}  // namespace stacksafe
