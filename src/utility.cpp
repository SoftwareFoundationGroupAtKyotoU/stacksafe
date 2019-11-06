#include "utility.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {

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
std::string to_alphabet(int num) {
  std::string ret;
  for (; 0 < num; num /= 26) {
    ret.insert(0, 1, 'a' + --num % 26);
  }
  return ret;
}
std::string to_str(const llvm::Type &type) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  type.print(stream, false, true);
  return stream.str();
}
std::string to_str(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << value;
  return stream.str();
}
std::string get_operand(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}
std::optional<int> register_number(const llvm::Value &value) {
  static const std::string prefix{"%"};
  auto operand = get_operand(value);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix) {
    return to_int(view.substr(1));
  }
  return std::nullopt;
}

bool check_voidfunc(const llvm::Value &v) {
  if (auto i = llvm::dyn_cast<llvm::CallInst>(&v)) {
    return i->getFunctionType()->getReturnType()->isVoidTy();
  }
  return false;
}
bool check_register(const llvm::Value &v) {
  if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    return !(i->isTerminator() || check_voidfunc(*i) ||
             llvm::isa<llvm::StoreInst>(v) || llvm::isa<llvm::FenceInst>(v));
  } else {
    return llvm::isa<llvm::Argument>(v);
  }
}
bool check_global(const llvm::Value &v) {
  if (llvm::isa<llvm::GlobalValue>(v)) {
    return true;
  } else if (auto c = llvm::dyn_cast<llvm::Constant>(&v)) {
    for (unsigned i = 0; i < c->getNumOperands(); ++i) {
      if (check_global(*c->getOperand(i))) {
        return true;
      }
    }
  }
  return false;
}
bool check_constant(const llvm::Value &v) {
  return llvm::isa<llvm::Constant>(v);
}

}  // namespace stacksafe
