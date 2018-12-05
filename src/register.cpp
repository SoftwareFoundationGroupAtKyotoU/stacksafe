#include "register.hpp"
#include "visualize.hpp"
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Value::Value(llvm::Value &v)
    : val_(&v), num_(get_number(v)) {
  }
  const llvm::Value &Value::get() const {
    return *val_;
  }
  bool Value::is_register() const {
    return 0 <= num_;
  }
  void Value::print(llvm::raw_ostream &O) const {
    if (is_register()) {
      O << angles(make_manip("%", num_));
    } else {
      O << *val_;
    }
  }

  Register::Register(llvm::Value &v)
    : Value(v) {
  }
  std::size_t Register::hash() const {
    return std::hash<void *>{}(val_);
  }
  bool Register::operator==(const Register &rhs) const {
    return val_ == rhs.val_;
  }
  std::optional<Register> make_register(llvm::Value &v) {
    auto reg = Register{v};
    if (reg.is_register()) {
      return reg;
    } else {
      return std::nullopt;
    }
  }

  int get_number(llvm::Value &v) {
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
}
namespace std {
  size_t hash<stacksafe::Register>::operator()(stacksafe::Register r) const {
    return r.hash();
  }
}
