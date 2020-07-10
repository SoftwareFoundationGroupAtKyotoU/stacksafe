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
Cell Cell::deref(const Cell& cell, int level) {
  return Cell{cell.value(), level + cell.level()};
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
  std::string buf;
  auto level = cell.level();
  for (; 0 < level; --level) {
    buf += "*";
  }
  buf += cell.to_string();
  j = buf;
}

Value::Value() = default;
Value::Value(const llvm::Value* v) {
  if (llvm::isa<llvm::Constant>(v)) {
    Super::insert(Cell::make(v));
  } else if (llvm::isa<llvm::Argument>(v)) {
    Super::insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::BinaryOperator>(v)) {
    insert(i->getOperand(0));
    insert(i->getOperand(1));
  } else if (llvm::isa<llvm::AllocaInst>(v)) {
    Super::insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::LoadInst>(v)) {
    for (const auto& cell : Value{i->getPointerOperand()}) {
      Super::insert(Cell::deref(cell, 1));
    }
  } else if (auto i = llvm::dyn_cast<llvm::GetElementPtrInst>(v)) {
    insert(i->getPointerOperand());
  } else if (auto i = llvm::dyn_cast<llvm::CastInst>(v)) {
    insert(i->getOperand(0));
  } else if (auto i = llvm::dyn_cast<llvm::PHINode>(v)) {
    for (const auto& use : i->incoming_values()) {
      insert(use.get());
    }
  } else if (auto i = llvm::dyn_cast<llvm::SelectInst>(v)) {
    insert(i->getTrueValue());
    insert(i->getFalseValue());
  } else {
    debug::print("unsupported value: " + debug::to_str(v));
  }
}
void Value::insert(const Value& value) {
  Super::insert(value.begin(), value.end());
}
bool Value::is_local() const {
  auto pred = [](const Cell& cell) { return cell.is_local(); };
  return std::any_of(begin(), end(), pred);
}
void Value::insert(const llvm::Value* value) {
  insert(Value{value});
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
