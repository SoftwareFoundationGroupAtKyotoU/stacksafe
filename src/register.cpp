#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Register::Register(int) : reg_{nullptr} {}
Register::Register(const llvm::Value &v) : reg_{&v} {}
int Register::number() const {
  if (reg_) {
    return register_number(*reg_).value_or(-2);
  } else {
    return -1;
  }
}
bool Register::less(const Register &that) const {
  return this->reg_ < that.reg_;
}
bool Register::is_local() const {
  return reg_ != nullptr;
}
const Register &Register::get_global() {
  static Register global{-1};
  return global;
}
bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.less(rhs);
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (reg.is_local()) {
    return prefix + std::to_string(reg.number());
  } else {
    return prefix + global;
  }
}

}  // namespace stacksafe
