#include "register.hpp"
#include "visualize.hpp"
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Register::Register(llvm::Value &v, int n)
    : reg_(&v), num_(n)
  {}
  const llvm::Value &Register::get() const {
    return *reg_;
  }
  std::size_t Register::hash() const {
    return std::hash<void *>{}(reg_);
  }
  bool Register::operator==(const Register &rhs) const {
    return reg_ == rhs.reg_;
  }
  void Register::print(llvm::raw_ostream &O) const {
    O << angles(make_manip("%", num_));
  }

  Value::Value(llvm::Value &v)
    : val_(&v), num_(get_number(v)) {
  }

  std::optional<Register> make_register(llvm::Value &v) {
    auto n = get_number(v);
    if (n < 0) {
      llvm::errs() << "unknown register" << colon << v << endl;
      return std::nullopt;
    } else {
      return Register(v, n);
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
