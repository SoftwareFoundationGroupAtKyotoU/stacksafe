#include "cell.hpp"
#include "utility.hpp"

namespace dataflow {
Cell::Cell(const llvm::Value* value, int level)
    : value_{value}, level_{level} {}
Cell Cell::make(const llvm::Value* value) {
  return Cell{value, 0};
}
Cell Cell::deref(const Cell& cell) {
  return Cell{cell.value(), 1 + cell.level()};
}
const llvm::Value* Cell::value() const {
  return value_;
}
int Cell::level() const {
  return level_;
}
std::string Cell::to_string() const {
  return debug::to_label(value_);
}
bool operator<(const Cell& lhs, const Cell& rhs) {
  return lhs.value() < rhs.value();
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
  buf += cell.to_string();
  j = buf;
}

void to_join(nlohmann::json& j, const Value& set) {
  std::vector<Cell> vec;
  for (const auto& cell : set) {
    vec.push_back(cell);
  }
  const auto cmp = [](const Cell& lhs, const Cell& rhs) {
    return lhs.to_string() < rhs.to_string();
  };
  std::sort(vec.begin(), vec.end(), cmp);
  j = vec;
}
}  // namespace dataflow
