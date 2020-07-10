#include "dataflow.hpp"
#include <llvm/IR/Function.h>
#include "block.hpp"
#include "function.hpp"
#include "utility.hpp"

namespace dataflow {
namespace {
using RegisterPass = llvm::RegisterPass<DataFlow>;
static RegisterPass dataflow{"dataflow", "dataflow", true, true};
}  // namespace

char DataFlow::ID = 0;
DataFlow::DataFlow() : llvm::ModulePass{ID} {}
bool DataFlow::runOnModule(llvm::Module &m) {
  const auto hr = "----------------";
  const auto hr2 = "================";
  for (const auto &c : function::Scc::solve(m)) {
    for (const auto &f : c) {
      debug::print(f->getName());
      for (const auto &c : block::Scc::solve(*f)) {
        for (const auto &b : c) {
          debug::print(debug::to_label(b));
        }
        debug::print(hr);
      }
    }
    debug::print(hr2);
  }
  return false;
}
void DataFlow::print(llvm::raw_ostream &, const llvm::Module *) const {}
}  // namespace dataflow
