#include "dataflow.hpp"

namespace dataflow {
namespace {
using RegisterPass = llvm::RegisterPass<DataFlow>;
static RegisterPass dataflow{"dataflow", "dataflow", true, true};
}  // namespace

char DataFlow::ID = 0;
DataFlow::DataFlow() : llvm::ModulePass{ID} {}
bool DataFlow::runOnModule(llvm::Module&) {
  return false;
}
void DataFlow::print(llvm::raw_ostream&, const llvm::Module*) const {}
}  // namespace dataflow
