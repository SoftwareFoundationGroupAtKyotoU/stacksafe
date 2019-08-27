#include "io.hpp"
#include <llvm/IR/Type.h>
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
template <typename T>
std::string do_to_string(const T& t) {
  auto f = [&t](llvm::raw_ostream& os) { os << t; };
  return string_from_stream(std::move(f));
}
}  // namespace

std::string get_operand(const llvm::Value& v, bool with_type) {
  auto f = [&v, with_type](llvm::raw_ostream& os) {
    v.printAsOperand(os, with_type);
  };
  return string_from_stream(std::move(f));
}
std::string to_string(const llvm::Value& v) { return do_to_string(v); }
std::string to_string(const llvm::Type& t) { return do_to_string(t); }
std::string type_tag(const llvm::Value& v) {
  std::string buf;
  if (auto t = v.getType()) {
    buf += "<";
    buf += to_string(*t);
    buf += ">";
  }
  return buf;
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
