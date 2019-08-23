#include "value.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
namespace value {
namespace {
int get_number(const llvm::Value& v) {
  auto digits = "0123456789";
  std::string str;
  llvm::raw_string_ostream ss{str};
  v.printAsOperand(ss, false);
  const auto& s = ss.str();
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string::npos) {
    return std::stoi(s.substr(1));
  } else {
    return -1;
  }
}
}  // namespace

Value::Value(const llvm::Value& v) : val_{&v}, num_{get_number(v)} {}
int Value::compare(const Value& that) const {
  if (num_ != that.num_) {
    return num_ - that.num_;
  } else if (num_ < 0) {
    return val_ - that.val_;
  } else {
    return 0;
  }
}
bool Value::is_register() const { return 0 <= num_; }
bool operator<(const Value& lhs, const Value& rhs) {
  return lhs.compare(rhs) < 0;
}
}  // namespace value
}  // namespace stacksafe
