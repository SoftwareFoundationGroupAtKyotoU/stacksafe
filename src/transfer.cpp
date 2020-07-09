#include "transfer.hpp"
#include "utility.hpp"

namespace dataflow {
Transfer::Transfer(const State &state) : state_{state} {}
auto Transfer::visitInstruction(llvm::Instruction &i) -> RetTy {
  debug::print("unsupported instruction: " + debug::to_str(i));
}
}  // namespace dataflow
