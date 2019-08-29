#include "type.hpp"
#include "io.hpp"
#include "json.hpp"

namespace stacksafe {

Type::Type(const llvm::Type* t) {
  if (t) {
    repr_ += "<";
    repr_ += to_string(*t);
    repr_ += ">";
  }
}
std::string Type::repr() const { return repr_; }
void to_json(Json& j, const Type& x) { j = x.repr(); }

}  // namespace stacksafe
