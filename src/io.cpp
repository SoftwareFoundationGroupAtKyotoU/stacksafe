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

}  // namespace stacksafe
