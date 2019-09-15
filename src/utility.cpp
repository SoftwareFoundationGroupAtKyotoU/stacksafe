#include "utility.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "value.hpp"

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
std::string to_str(const llvm::Type& type) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  type.print(stream, false, true);
  return stream.str();
}
std::string to_str(const llvm::Value& value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << value;
  return stream.str();
}
std::string to_str(const Value& value) { return to_str(*value.get()); }
std::string get_operand(const llvm::Value& value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}

void endline(llvm::raw_ostream& os) { (os << "\n").flush(); }
void print_string(llvm::raw_ostream& os, const std::string& str) { os << str; }
void print_stdout(const std::string& str) {
  print_string(llvm::outs(), str);
  endline(llvm::outs());
}
bool is_voidfunc(const llvm::Value& v) {
  if (auto i = llvm::dyn_cast<llvm::CallInst>(&v)) {
    return i->getFunctionType()->getReturnType()->isVoidTy();
  }
  return false;
}
bool is_register(const llvm::Value& v) {
  if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    return !i->isTerminator() && !is_voidfunc(*i);
  } else {
    return llvm::isa<llvm::Argument>(v);
  }
}
bool is_global(const llvm::Value& v) {
  if (llvm::isa<llvm::GlobalValue>(v)) {
    return true;
  } else if (llvm::isa<llvm::Constant>(v)) {
    return false;
  } else {
    stacksafe_unreachable("neither global nor constant", v);
  }
}
bool check_constant(const llvm::Value& v) {
  return llvm::isa<llvm::Constant>(v);
}
bool check_register(const llvm::Value& v) {
  if (auto i = llvm::dyn_cast<llvm::CallInst>(&v)) {
    return !i->getFunctionType()->getReturnType()->isVoidTy();
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    return !i->isTerminator();
  } else {
    return llvm::isa<llvm::Argument>(v);
  }
}
void unknown_instruction(const llvm::Instruction& i) {
  endline(llvm::errs() << "Unknown instruction: " << i);
}
void check_interpret(const llvm::Instruction& i, bool success) {
  if (!success) {
    endline(llvm::errs() << "Failed to interpret: " << i);
  }
}

}  // namespace stacksafe
