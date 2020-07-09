#include "cell.hpp"

namespace dataflow {
Cell::Cell(const llvm::Value* value, int level)
    : value_{value}, level_{level} {}
const llvm::Value* Cell::value() const {
  return value_;
}
int Cell::level() const {
  return level_;
}
}  // namespace dataflow
