#include "type.hpp"
#include <llvm/IR/Type.h>
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

Type::Type(const llvm::Type* t) : type_{t} {}
const llvm::Type* Type::get() const { return type_; }
std::string Type::repr() const {
  if (type_) {
    return to_str(*type_);
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
void to_json(Json& j, const Type& x) { j = "<" + x.repr() + ">"; }
Fabric dump(const Type& type) {
  Fabric ret;
  if (type.get()) {
    ret.append(type.repr());
  }
  return ret;
}

}  // namespace stacksafe
