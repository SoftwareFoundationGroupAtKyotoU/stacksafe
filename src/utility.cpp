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
bool check_voidfunc(const llvm::Value& v) {
  if (auto i = llvm::dyn_cast<llvm::CallInst>(&v)) {
    return i->getFunctionType()->getReturnType()->isVoidTy();
  }
  return false;
}
bool check_register(const llvm::Value& v) {
  if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    return !i->isTerminator() && !check_voidfunc(*i);
  } else {
    return llvm::isa<llvm::Argument>(v);
  }
}
bool check_global(const llvm::Value& v) {
  auto type = v.getType();
  if (auto c = llvm::dyn_cast<llvm::Constant>(&v); c && type) {
    if (llvm::isa<llvm::GlobalValue>(c)) {
      return true;
    } else if (auto a = llvm::dyn_cast<llvm::ConstantAggregate>(c)) {
      endline(llvm::errs() << "Aggregate: " << *a);
      return false;
    } else if (auto d = llvm::dyn_cast<llvm::ConstantData>(c)) {
      if (type->isIntegerTy() || type->isFloatingPointTy()) {
        return false;
      }
      endline(llvm::errs() << "Data: " << *d);
      return false;
    } else if (auto e = llvm::dyn_cast<llvm::ConstantExpr>(c)) {
      bool ret = false;
      for (unsigned i = 0; i < e->getNumOperands(); ++i) {
        auto op = e->getOperand(i);
        if (check_global(*op)) {
          ret = true;
        }
      }
      if (!ret) {
        endline(llvm::errs() << "Expr: " << *e);
      }
      return ret;
    }
    stacksafe_unreachable("unknown constant", *c);
  }
  stacksafe_unreachable("check_global", v);
}
bool check_constant(const llvm::Value& v) {
  return llvm::isa<llvm::Constant>(v);
}

}  // namespace stacksafe
