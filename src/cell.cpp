#include "cell.hpp"

namespace dataflow {
Cell::Cell(const llvm::Value* value, int level)
    : value_{value}, level_{level} {}
}  // namespace dataflow
