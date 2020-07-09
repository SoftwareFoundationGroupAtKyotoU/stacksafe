#include "transfer.hpp"
#include "state.hpp"
#include "utility.hpp"

namespace dataflow {
Transfer::Transfer(State &state) : state_{state} {}
auto Transfer::visitInstruction(llvm::Instruction &i) -> RetTy {
  debug::print("unsupported instruction: " + debug::to_str(i));
}
}  // namespace dataflow
