#include "utility.hpp"
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
std::string to_str(const llvm::Type& type) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  type.print(stream, false, true);
  return stream.str();
}
}  // namespace stacksafe
