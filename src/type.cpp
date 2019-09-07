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
bool Type::is_pointer() const {
  if (type_) {
    return type_->isPointerTy();
  }
  return false;
}
Type Type::pointee_type() const {
  return Type{is_pointer() ? type_->getPointerElementType() : nullptr};
}
void to_json(Json& j, const Type& x) { j = x.repr(); }

Token::Token(int n, const Type& t) : num_{n}, type_{t} {}

}  // namespace stacksafe
