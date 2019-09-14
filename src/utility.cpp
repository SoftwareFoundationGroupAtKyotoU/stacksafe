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
std::string get_operand(const llvm::Value& value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}

void print_string(llvm::raw_ostream& os, const std::string& str) {
  os << str << "\n";
}
void print_stdout(const std::string& str) { print_string(llvm::outs(), str); }
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
  llvm::errs() << "Unknown instruction: " << i << "\n";
}
void check_interpret(const llvm::Instruction& i, bool success) {
  if (!success) {
    llvm::errs() << "Failed to interpret: " << i << "\n";
  }
}

}  // namespace stacksafe
