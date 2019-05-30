#include "register.hpp"
#include "visualize.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace stacksafe {
Value::Value(const llvm::Value &v) : val_(&v), num_(get_number(v)) {}
const llvm::Value &Value::get() const { return *val_; }
std::size_t Value::hash() const { return std::hash<const void *>{}(val_); }
bool Value::operator==(const Value &rhs) const { return val_ == rhs.val_; }
bool Value::is_register() const { return 0 <= num_; }
void Value::print(llvm::raw_ostream &O) const {
  if (is_register()) {
    O << angles(manip("%", num_));
  } else {
    O << *val_;
  }
}

Register::Register(const llvm::Value &v) : Value(v) {}
std::optional<Register> make_register(const llvm::Value &v) {
  auto reg = Register{v};
  if (reg.is_register()) {
    return reg;
  } else {
    return std::nullopt;
  }
}
std::optional<Register> make_register(const Value &v) {
  return make_register(v.get());
}

int get_number(const llvm::Value &v) {
  auto digits = "0123456789";
  std::string str;
  llvm::raw_string_ostream ss{str};
  v.printAsOperand(ss, false);
  const auto &s = ss.str();
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string::npos) {
    return std::stoi(s.substr(1));
  } else {
    return -1;
  }
}
} // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value &v) const {
  return v.hash();
}
size_t hash<stacksafe::Register>::
operator()(const stacksafe::Register &r) const {
  return r.hash();
}
} // namespace std
