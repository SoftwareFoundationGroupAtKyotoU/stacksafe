#include "cell.hpp"
#include <llvm/IR/Instructions.h>
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

Value::Value() = default;
Value::Value(const llvm::Value* v) {
  if (llvm::isa<llvm::Argument>(v)) {
    Super::insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::BinaryOperator>(v)) {
    insert(i->getOperand(0));
    insert(i->getOperand(1));
  } else if (llvm::isa<llvm::AllocaInst>(v)) {
    Super::insert(Cell::make(v));
  } else if (auto i = llvm::dyn_cast<llvm::LoadInst>(v)) {
    Value src{i->getPointerOperand()};
    for (const auto& cell : src) {
      Super::insert(Cell::deref(cell));
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
  } else if (v) {
    debug::print("unsupported value: " + debug::to_str(*v));
  } else {
    debug::print("null value");
  }
}
void Value::insert(const Value& value) {
  Super::insert(value.begin(), value.end());
}
void Value::insert(const llvm::Value* value) {
  insert(Value{value});
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
