#include "register.hpp"
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>
#include "io.hpp"
#include "json.hpp"

namespace stacksafe {

Register::Register(const llvm::Value& v)
    : num_{-1}, val_{v}, type_{v.getType()} {
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
const Type& Register::get_type() const { return type_; }
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
    str += x.get_type().repr();
    j = str;
  } else {
    j = to_string(x.get_val());
  }
}

}  // namespace stacksafe
