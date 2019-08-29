#ifndef INCLUDE_GUARD_AAFE405E_E57A_4417_B712_250BD5954A09
#define INCLUDE_GUARD_AAFE405E_E57A_4417_B712_250BD5954A09

#include <optional>
#include <string>
#include <string_view>

namespace llvm {
class Value;
}  // namespace llvm

namespace stacksafe {

std::string get_operand(const llvm::Value& v, bool with_type = false);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_AAFE405E_E57A_4417_B712_250BD5954A09
