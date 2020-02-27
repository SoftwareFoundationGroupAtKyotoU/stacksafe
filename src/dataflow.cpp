#include "dataflow.hpp"

namespace stacksafe {

DataFlow::DataFlow(const llvm::Function& f) : func_{f} {}

}  // namespace stacksafe
