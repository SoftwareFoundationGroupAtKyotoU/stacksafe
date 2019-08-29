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
}  // namespace

Value::Value(const llvm::Value& v) : type_{v.getType()} {
  auto prefix = "%";
  auto operand = get_operand(v);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix) {
    num_ = to_int(view.substr(1));
  }
  if (is_register()) {
    repr_ = prefix + std::to_string(*num_) + type_.repr();
  } else {
    repr_ = to_str(v);
  }
}
std::optional<int> Value::number() const { return num_; }
const Type& Value::type() const { return type_; }
std::string Value::repr() const { return repr_; }
bool Value::is_register() const { return num_.has_value(); }
bool operator<(const Value& lhs, const Value& rhs) {
  auto l = lhs.number().value_or(-1);
  auto r = rhs.number().value_or(-1);
  if (l == r) {
    return lhs.repr() < rhs.repr();
  } else {
    return l < r;
  }
}
void to_json(Json& j, const Value& x) { j = x.repr(); }

}  // namespace stacksafe