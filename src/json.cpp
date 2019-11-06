#include "json.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "value.hpp"

namespace stacksafe {

std::string get_operand(const Value& v) {
  assert(v && "no operand at nullptr");
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v.get()->printAsOperand(stream, false);
  return stream.str();
}

}  // namespace stacksafe
