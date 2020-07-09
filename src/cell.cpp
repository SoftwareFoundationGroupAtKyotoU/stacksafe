#include "cell.hpp"
#include "utility.hpp"

namespace dataflow {
Cell::Cell(const llvm::Value* value, int level)
    : value_{value}, level_{level} {}
Cell Cell::make(const llvm::Value* value) {
  return Cell{value, 0};
}
const llvm::Value* Cell::value() const {
  return value_;
}
int Cell::level() const {
  return level_;
}
void to_json(nlohmann::json& j, const Cell& cell) {
  std::string buf;
  auto level = cell.level();
  for (; 0 < level; --level) {
    buf += "*";
  }
  for (; level < 0; ++level) {
    buf += "&";
  }
  buf += debug::to_label(cell.value());
  j = buf;
}
}  // namespace dataflow
