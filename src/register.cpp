#include "register.hpp"
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>
#include "io.hpp"

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
}  // namespace

Register::Register(const llvm::Value& v) : num_{-1}, repr_{"%"}, val_{v} {
  const auto digits = "0123456789";
  std::string_view s{get_operand(v)};
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string_view::npos) {
    if (auto n = to_int(s.substr(1))) {
      num_ = *n;
    }
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
