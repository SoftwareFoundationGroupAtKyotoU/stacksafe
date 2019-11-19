#include "utility.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include "value.hpp"

namespace stacksafe {

void unsupported_instruction(const llvm::Instruction& i) {
  static const auto msg = "FATAL ERROR: unsupported instruction: ";
  (llvm::errs() << msg << i << "\n").flush();
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

namespace debug {
int get_operand(const Value& v) {
  static const auto prefix = '%';
  if (!v) {
    return -1;
  }
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v.get()->printAsOperand(stream, false);
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
}  // namespace debug
}  // namespace stacksafe
