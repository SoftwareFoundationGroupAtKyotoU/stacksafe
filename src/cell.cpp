#include "cell.hpp"
#include <llvm/IR/Instructions.h>
#include <algorithm>
#include "utility.hpp"

namespace dataflow {
Cell::Cell(const llvm::Value* value, int level)
    : value_{value}, level_{level} {}
Cell Cell::make(const llvm::Value* value) {
  if (!llvm::isa<llvm::Constant>(value) && !llvm::isa<llvm::Argument>(value) &&
      !llvm::isa<llvm::AllocaInst>(value)) {
    debug::print("invalid cell: " + debug::to_str(value));
  }
  return Cell{value, 0};
}
Cell Cell::shift(const Cell& cell, int level) {
  level += cell.level();
  assert(0 <= level);
  return Cell{cell.value(), level};
}
const llvm::Value* Cell::value() const {
  return value_;
}
int Cell::level() const {
  return level_;
}
std::string Cell::to_string() const {
  return std::string(level_, '*') + debug::to_label(value_);
}
bool Cell::is_local() const {
  return llvm::isa<llvm::AllocaInst>(value_);
}
bool operator<(const Cell& lhs, const Cell& rhs) {
  if (lhs.value() == rhs.value()) {
    return lhs.level() < rhs.level();
  } else {
    return lhs.value() < rhs.value();
  }
}
void to_json(nlohmann::json& j, const Cell& cell) {
  j = cell.to_string();
}

Value::Value() = default;
Value Value::make(const llvm::Value* v) {
  Value value;
  if (llvm::isa<llvm::Constant>(v)) {
    value.insert(Cell::make(v));
  } else if (llvm::isa<llvm::Argument>(v)) {
    value.insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::BinaryOperator>(v)) {
    value.insert(Value::make(i->getOperand(0)));
    value.insert(Value::make(i->getOperand(1)));
  } else if (llvm::isa<llvm::AllocaInst>(v)) {
    value.insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::LoadInst>(v)) {
    for (const auto& cell : Value::make(i->getPointerOperand())) {
      value.insert(Cell::shift(cell, 1));
    }
  } else if (auto i = llvm::dyn_cast<llvm::GetElementPtrInst>(v)) {
    value.insert(Value::make(i->getPointerOperand()));
  } else if (auto i = llvm::dyn_cast<llvm::CastInst>(v)) {
    value.insert(Value::make(i->getOperand(0)));
  } else if (auto i = llvm::dyn_cast<llvm::PHINode>(v)) {
    for (const auto& use : i->incoming_values()) {
      value.insert(Value::make(use.get()));
    }
  } else if (auto i = llvm::dyn_cast<llvm::SelectInst>(v)) {
    value.insert(Value::make(i->getTrueValue()));
    value.insert(Value::make(i->getFalseValue()));
  } else {
    debug::print("unsupported value: " + debug::to_str(v));
  }
  return value;
}
void Value::insert(const Value& value) {
  Super::insert(value.begin(), value.end());
}
bool Value::is_local() const {
  auto pred = [](const Cell& cell) { return cell.is_local(); };
  return std::any_of(begin(), end(), pred);
}
bool Value::is_global() const {
  auto pred = [](const Cell& cell) { return !cell.is_local(); };
  return std::any_of(begin(), end(), pred);
}
void to_json(nlohmann::json& j, const Value& value) {
  std::vector<Cell> vec;
  for (const auto& cell : value) {
    vec.push_back(cell);
  }
  const auto cmp = [](const Cell& lhs, const Cell& rhs) {
    return lhs.to_string() < rhs.to_string();
  };
  std::sort(vec.begin(), vec.end(), cmp);
  j = vec;
}
}  // namespace dataflow
