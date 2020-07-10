#include "utility.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {

std::size_t operator"" _z(unsigned long long n) {
  return n;
}
std::string to_str(const llvm::Value &v) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << v;
  return stream.str();
}
void fatal_error(const std::string &msg) {
  debug::print("FATAL: " + msg);
  std::exit(EXIT_FAILURE);
}
bool least_significant_bit(const void *ptr) {
  return reinterpret_cast<std::uintptr_t>(ptr) & 0x1;
}
bool is_global(const llvm::Value &v) {
  if (const auto c = llvm::dyn_cast<llvm::Constant>(&v)) {
    if (llvm::isa<llvm::GlobalValue>(c)) {
      return true;
    }
    for (unsigned i = 0; i < c->getNumOperands(); ++i) {
      if (is_global(*c->getOperand(i))) {
        return true;
      }
    }
  }
  return false;
}
bool is_register(const llvm::Value &v) {
  if (const auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    if (const auto c = llvm::dyn_cast<llvm::CallInst>(i)) {
      return !c->getFunctionType()->getReturnType()->isVoidTy();
    } else if (i->isTerminator()) {
      return false;
    } else if (llvm::isa<llvm::StoreInst>(i)) {
      return false;
    } else if (llvm::isa<llvm::FenceInst>(i)) {
      return false;
    } else {
      return true;
    }
  } else if (llvm::isa<llvm::Argument>(v)) {
    return true;
  } else {
    return false;
  }
}
bool is_return(const llvm::CallInst &i) {
  return !i.getFunctionType()->getReturnType()->isVoidTy();
}

namespace debug {
int get_operand(const llvm::Value *v) {
  static const auto prefix = '%';
  if (!v) {
    return -1;
  }
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v->printAsOperand(stream, false);
  std::string_view view{stream.str()};
  if (!view.empty() && view.at(0) == prefix) {
    std::string str{view.substr(1)};
    std::size_t pos = std::string::npos;
    auto val = std::stoi(str, &pos, 10);
    if (pos == str.size()) {
      return (pos < 0) ? -2 : val;
    }
    return -3;
  }
  return -4;
}
void print(const std::string &str) {
  (llvm::errs() << str << "\n").flush();
}
std::string to_str(int num) {
  switch (num) {
    case -1:
      return "@";
    case -2:
      return "negative number";
    case -3:
      return "not integer";
    case -4:
      return "invalid register";
    default:
      return num < 0 ? "unknown" : std::to_string(num);
  }
}
}  // namespace debug
}  // namespace stacksafe

namespace dataflow {
namespace debug {
void print(const std::string &str) {
  (llvm::errs() << str << "\n").flush();
}
std::string to_str(const llvm::Value *v) {
  if (v) {
    std::string buf;
    llvm::raw_string_ostream stream{buf};
    stream << *v;
    return stream.str();
  }
  return "null";
}
std::string to_label(const llvm::Value *v) {
  if (v) {
    std::string buf;
    llvm::raw_string_ostream stream{buf};
    v->printAsOperand(stream, false);
    return stream.str();
  }
  return "null";
}
}  // namespace debug
}  // namespace dataflow
