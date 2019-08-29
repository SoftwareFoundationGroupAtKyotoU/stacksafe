#include "register.hpp"
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>
#include "io.hpp"
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

Register::Register(const llvm::Value& v)
    : num_{-1}, val_{v}, type_{v.getType()}, repr_{"%"} {
  const auto digits = "0123456789";
  std::string_view s{get_operand(v)};
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string_view::npos) {
    if (auto n = to_int(s.substr(1))) {
      num_ = *n;
      repr_ += std::to_string(num_) + type_.repr();
      return;
    }
  }
  repr_ = to_str(v);
}
int Register::get_num() const { return num_; }
const llvm::Value& Register::get_val() const { return val_; }
const Type& Register::get_type() const { return type_; }
std::string Register::repr() const { return repr_; }
bool Register::is_register() const { return 0 <= num_; }
bool operator<(const Register& lhs, const Register& rhs) {
  auto l = lhs.get_num();
  auto r = rhs.get_num();
  if (l == r) {
    return &lhs.get_val() < &rhs.get_val();
  } else {
    return l < r;
  }
}
void to_json(Json& j, const Register& x) { j = x.repr(); }

}  // namespace stacksafe
