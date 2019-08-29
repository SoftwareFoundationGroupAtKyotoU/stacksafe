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

Type::Type(const llvm::Type* t) : type_{t} {}
const llvm::Type* Type::get() const { return type_; }
std::string Type::repr() const {
  if (type_) {
    return "<" + to_str(*type_) + ">";
  }
  return {};
}
void to_json(Json& j, const Type& x) { j = x.repr(); }

}  // namespace stacksafe
