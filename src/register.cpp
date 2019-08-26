#include "register.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string>
#include <string_view>

namespace {
std::string get_operand(const llvm::Value& v, bool with_type = false) {
  std::string str;
  llvm::raw_string_ostream ss{str};
  v.printAsOperand(ss, with_type);
  return ss.str();
}
std::string to_string(const llvm::Value& v) {
  std::string str;
  llvm::raw_string_ostream ss{str};
  v.print(ss);
  return ss.str();
}
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
std::optional<int> to_number(const llvm::Value& v) {
  const auto digits = "0123456789";
  auto operand = get_operand(v);
  std::string_view s{operand};
  if (!s.empty() && s.front() == '%' &&
      s.find_first_not_of(digits, 1) == std::string_view::npos) {
    return to_int(s.substr(1));
  }
  return std::nullopt;
}
}  // namespace
