#include "io.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
namespace {
template <typename F>
std::string string_from_stream(F f) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  f(stream);
  return stream.str();
}
}  // namespace

std::string get_operand(const llvm::Value& v, bool with_type) {
  auto f = [&v, with_type](llvm::raw_ostream& os) {
    v.printAsOperand(os, with_type);
  };
  return string_from_stream(std::move(f));
}
std::string to_string(const llvm::Value& v) {
  auto f = [&v](llvm::raw_ostream& os) { os << v; };
  return string_from_stream(std::move(f));
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

}  // namespace stacksafe
