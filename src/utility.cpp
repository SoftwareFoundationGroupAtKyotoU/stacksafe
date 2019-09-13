#include "utility.hpp"
#include <llvm/IR/Constants.h>
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

void constant_info(const llvm::Value& v) {
  if (llvm::isa<llvm::Constant>(v)) {
    auto tag = llvm::isa<llvm::ConstantAggregate>(v) ?
                   "aggregate" :
                   llvm::isa<llvm::ConstantData>(v) ?
                   "data" :
                   llvm::isa<llvm::ConstantExpr>(v) ?
                   "expr" :
                   llvm::isa<llvm::GlobalValue>(v) ? "global" : "constant";
    llvm::errs() << tag << ": " << v << "\n";
  }
}

}  // namespace stacksafe
