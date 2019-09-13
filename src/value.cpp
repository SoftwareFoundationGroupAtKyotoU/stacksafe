#include "value.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {
namespace {
std::optional<int> to_int(std::string_view view) {
  if (!view.empty()) {
    std::string str{view};
    std::size_t pos = std::string_view::npos;
    auto val = std::stoi(str, &pos, 10);
    if (pos == view.size()) {
      return val;
    }
  }
  return std::nullopt;
}
std::string to_str(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << value;
  return stream.str();
}
std::string get_operand(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}
}  // namespace

const std::string Value::prefix_{"%"};
Value::Value(int n, llvm::Type *t, const llvm::Value *v)
    : Token{n, Type{t}}, value_{v} {}
Value Value::create(const llvm::Value &v) {
  int num = -1;
  auto operand = get_operand(v);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix_) {
    if (auto i = to_int(view.substr(1))) {
      num = *i;
    }
  }
  return Value{num, v.getType(), &v};
}
Value::Value(int n) : Value{n, nullptr, nullptr} {}
const llvm::Value *Value::get() const { return value_; }
std::string Value::repr() const {
  if (is_register()) {
    return prefix_ + std::to_string(number());
  } else {
    return to_str(*value_);
  }
}
bool Value::is_register() const { return 0 <= number(); }
bool operator<(const Value &lhs, const Value &rhs) {
  if (lhs.is_register() || rhs.is_register()) {
    return lhs.number() < rhs.number();
  } else {
    return lhs.repr() < rhs.repr();
  }
}
void to_json(Json &j, const Value &x) { j = x.repr(); }
Fabric dump(const Value &value) {
  Fabric ret;
  return ret.append(value.repr()).append(dump(value.type())).quote();
}

}  // namespace stacksafe
