#include "type.hpp"
#include <llvm/IR/Type.h>
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"

namespace stacksafe {
namespace {
std::string to_str(const llvm::Type& type) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << type;
  return stream.str();
}
}  // namespace

Type::Type(const llvm::Type* t) {
  if (t) {
    repr_ += "<";
    repr_ += to_str(*t);
    repr_ += ">";
  }
}
std::string Type::repr() const { return repr_; }
void to_json(Json& j, const Type& x) { j = x.repr(); }

}  // namespace stacksafe
