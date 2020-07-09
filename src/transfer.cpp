#include "transfer.hpp"
#include "utility.hpp"

namespace dataflow {
auto Transfer::visitInstruction(llvm::Instruction &i) -> RetTy {
  debug::print("unsupported instruction: " + debug::to_str(i));
}
}  // namespace dataflow
