#include "utility.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "value.hpp"

namespace stacksafe {

std::string to_str(const Value& v) {
  assert(v && "no operand at nullptr");
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v.get()->print(stream);
  return stream.str();
}

}  // namespace stacksafe
