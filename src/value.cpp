#include "value.hpp"

namespace stacksafe {

Value::Value() : val_{nullptr} {}
Value::Value(const llvm::Value &v) : val_{&v} {}
Value::operator bool() const {
  return val_ != nullptr;
}
const llvm::Value *Value::get() const {
  return val_;
}
std::size_t Value::hash(const Value &v) {
  return std::hash<const llvm::Value *>{}(v.val_);
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
  return stacksafe::Value::hash(val);
}
}  // namespace std
