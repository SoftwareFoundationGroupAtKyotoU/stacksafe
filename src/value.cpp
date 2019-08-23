#include "value.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"

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
std::string to_string(const llvm::Value& v) {
  std::string buf;
  llvm::raw_string_ostream os{buf};
  os << v;
  return os.str();
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
std::string Value::to_string() const {
  using namespace std::string_literals;
  if (is_register()) {
    return "%"s + std::to_string(num_);
  } else {
    return value::to_string(*val_);
  }
}
bool operator<(const Value& lhs, const Value& rhs) {
  return lhs.compare(rhs) < 0;
}
Json to_json(const Value& v) { return Json{v.to_string()}; }
}  // namespace value
}  // namespace stacksafe
