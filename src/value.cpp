#include "value.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {
namespace {}  // namespace

Value::Value() : val_{nullptr} {}
Value::Value(const llvm::Value &v) : val_{&v} {}
Value::operator bool() const {
  return val_ != nullptr;
}
const llvm::Value *Value::get() const {
  return val_;
}
std::size_t Value::hash() const {
  return std::hash<const llvm::Value *>{}(val_);
}
auto Value::kind() const -> Kind {
  const auto v = get();
  if (!v) {
    return Kind::GLOBAL;
  } else if (llvm::isa<llvm::Argument>(v)) {
    return Kind::ARGUMENT;
  } else if (auto c = llvm::dyn_cast<llvm::Constant>(v)) {
    return is_global(*c) ? Kind::GLOBAL : Kind::CONSTANT;
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(v)) {
    return is_register(*i) ? Kind::REGISTER : Kind::OTHER;
  } else {
    return Kind::OTHER;
  }
}
bool operator<(const Value &lhs, const Value &rhs) {
  return lhs.get() < rhs.get();
}
bool operator==(const Value &lhs, const Value &rhs) {
  return lhs.get() == rhs.get();
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value &val) const {
  return val.hash();
}
}  // namespace std
