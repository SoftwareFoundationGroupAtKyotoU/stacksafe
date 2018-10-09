#include "register.hpp"
#include <string>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Register::Register(llvm::Value &value)
    : reg_(&value)
  {}
  std::size_t Register::hash() const {
    return std::hash<void *>{}(reg_);
  }
  int get_number(llvm::Value &v) {
    if (auto A = llvm::dyn_cast<llvm::Argument>(&v)) {
      return A->getArgNo();
    } else if (auto I = llvm::dyn_cast<llvm::Instruction>(&v)) {
      std::string str;
      llvm::raw_string_ostream ss(str);
      ss << I;
      auto b = str.find_first_not_of(" ");
      if (b == std::string::npos || str.at(b) != '%') {
        return -1;
      }
      ++b;
      auto e = str.find_first_not_of("0123456789", b);
      if (e == std::string::npos || str.at(e) != ' ' || b == e) {
        return -1;
      }
      return std::stoi(str.substr(b, e - b));
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
