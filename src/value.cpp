#include "value.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <string_view>
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
std::string to_str(const llvm::Value& value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << value;
  return stream.str();
}
std::string get_operand(const llvm::Value& value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}
std::optional<int> extract_num(const llvm::Value& value,
                               std::string_view prefix) {
  auto operand = get_operand(value);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix) {
    return to_int(view.substr(1));
  }
  return std::nullopt;
}
}  // namespace

const std::string Value::prefix_{"%"};
Value::Value(const llvm::Value& v)
    : value_{&v}, num_{extract_num(v, prefix_)}, type_{v.getType()} {}
Value::Value(int n) : value_{nullptr}, num_{n}, type_{nullptr} {}
std::optional<int> Value::number() const { return num_; }
const Type& Value::type() const { return type_; }
std::string Value::repr(std::size_t width) const {
  std::string ret;
  if (is_register()) {
    ret.insert(0, length() < width ? width - length() : 0, '0');
    ret.append(std::to_string(*num_));
    ret.append(type_.repr());
    ret.insert(0, prefix_);
  } else {
    ret = to_str(*value_);
  }
  return ret;
}
bool Value::is_register() const { return num_.has_value(); }
std::size_t Value::length() const {
  return num_ ? std::to_string(*num_).size() : 0;
}
bool operator<(const Value& lhs, const Value& rhs) {
  auto l = lhs.number().value_or(-1);
  auto r = rhs.number().value_or(-1);
  if (l == -1 && r == -1) {
    return lhs.repr() < rhs.repr();
  } else {
    return l < r;
  }
}
void to_json(Json& j, const Value& x) { j = x.repr(); }

}  // namespace stacksafe
