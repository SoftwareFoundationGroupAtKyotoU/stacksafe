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
std::string to_str(const Value &value) {
  if (auto v = value.value()) {
    return to_str(*v);
  } else {
    return "nullptr";
  }
}
std::string get_operand(const Value &value) {
  if (auto v = value.value()) {
    std::string buf;
    llvm::raw_string_ostream stream{buf};
    v->printAsOperand(stream, false);
    return stream.str();
  } else {
    return "nullptr";
  }
}

}  // namespace stacksafe
