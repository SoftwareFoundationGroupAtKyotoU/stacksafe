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
int Value::number() const { return num_.value_or(-1); }
const Type& Value::type() const { return type_; }
std::string Value::repr() const {
  if (is_register()) {
    return prefix_ + std::to_string(number());
  } else {
    return to_str(*value_);
  }
}
bool Value::is_register() const { return 0 <= number(); }
bool operator<(const Value& lhs, const Value& rhs) {
  auto l = lhs.number();
  auto r = rhs.number();
  if (l == -1 && r == -1) {
    return lhs.repr() < rhs.repr();
  } else {
    return l < r;
  }
}
void to_json(Json& j, const Value& x) { j = x.repr(); }

}  // namespace stacksafe
