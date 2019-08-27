#include "register.hpp"
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>

namespace stacksafe {
namespace {
template <typename F>
std::string string_from_stream(F f) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  f(stream);
  return stream.str();
}
template <typename T>
std::string to_string(const T& v) {
  auto f = [&v](llvm::raw_ostream& os) { os << v; };
  return string_from_stream(std::move(f));
}
std::string get_operand(const llvm::Value& v, bool with_type = false) {
  auto f = [&v, with_type](llvm::raw_ostream& os) {
    v.printAsOperand(os, with_type);
  };
  return string_from_stream(std::move(f));
}
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
std::optional<int> to_number(const llvm::Value& v) {
  const auto digits = "0123456789";
  auto operand = get_operand(v);
  std::string_view s{operand};
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string_view::npos) {
    return to_int(s.substr(1));
  }
  return std::nullopt;
}
}  // namespace

Register::Register(const llvm::Value& v) : num_{-1}, repr_{"%"}, val_{v} {
  if (auto n = to_number(v)) {
    num_ = *n;
    repr_ += std::to_string(num_);
    if (auto t = v.getType()) {
      repr_ += "<";
      repr_ += to_string(*t);
      repr_ += ">";
    }
  } else {
    repr_ = to_string(v);
  }
}
int Register::get_num() const { return num_; }
const llvm::Value& Register::get_val() const { return val_; }
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
void to_json(Json& j, const Register& x) {
  if (x.is_register()) {
    std::string str{"%"};
    str += std::to_string(x.get_num());
    if (auto t = x.get_val().getType()) {
      str += "<";
      str += to_string(*t);
      str += ">";
    }
    j = str;
  } else {
    j = to_string(x.get_val());
  }
}

}  // namespace stacksafe
