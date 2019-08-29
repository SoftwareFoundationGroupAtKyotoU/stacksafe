#include "type.hpp"
#include "io.hpp"

namespace stacksafe {

Type::Type(llvm::Type* t) {
  if (t) {
    repr_ += "<";
    repr_ += to_string(*t);
    repr_ += ">";
  }
}

}  // namespace stacksafe
