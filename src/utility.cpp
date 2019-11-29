#include "utility.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "domain.hpp"
#include "map.hpp"

namespace stacksafe {

std::string to_str(const llvm::Value& v) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << v;
  return stream.str();
}
void fatal_error(const std::string& msg) {
  debug::print("FATAL: " + msg);
  std::exit(EXIT_FAILURE);
}
bool is_global(const llvm::Constant& c) {
  if (llvm::isa<llvm::GlobalValue>(&c)) {
    return true;
  }
  for (unsigned i = 0; i < c.getNumOperands(); ++i) {
    assert(llvm::isa<llvm::Constant>(c.getOperand(i)) &&
           "constant has non-constant");
    if (auto v = llvm::dyn_cast<llvm::Constant>(c.getOperand(i));
        is_global(*v)) {
      return true;
    }
  }
  return false;
}
bool is_register(const llvm::Instruction& i) {
  if (auto c = llvm::dyn_cast<llvm::CallInst>(&i)) {
    return !c->getFunctionType()->getReturnType()->isVoidTy();
  } else if (i.isTerminator()) {
    return false;
  } else if (llvm::isa<llvm::StoreInst>(i)) {
    return false;
  } else if (llvm::isa<llvm::FenceInst>(i)) {
    return false;
  } else {
    return true;
  }
}
bool is_return(const llvm::CallInst& i) {
  return !i.getFunctionType()->getReturnType()->isVoidTy();
}

namespace debug {
int get_operand(const llvm::Value* v) {
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
void print(const std::string& str) {
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
std::string to_str(const Domain& dom) {
  std::string ret;
  std::vector<int> nums;
  for (const auto& sym : dom) {
    nums.push_back(get_operand(sym.value()));
  }
  std::sort(nums.begin(), nums.end());
  ret.append("[");
  for (const auto& num : nums) {
    ret.append(to_str(num));
    ret.append(", ");
  }
  ret.append("]");
  return ret;
}
std::string to_str(const Map& map) {
  std::string ret;
  for (const auto& key : Map::keys(map)) {
    ret.append(to_str(get_operand(key.value())));
    ret.append(": ");
    ret.append(to_str(map.lookup(key)));
    ret.append(", ");
  }
  return ret;
}
}  // namespace debug
}  // namespace stacksafe
