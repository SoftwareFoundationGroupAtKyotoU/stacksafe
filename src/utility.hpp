#ifndef INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
#define INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B

#include <cassert>
#include <optional>
#include <string>
#include <string_view>

namespace llvm {
class Type;
class Value;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Value;

std::optional<int> to_int(std::string_view view);
std::string to_alphabet(int num);
std::string to_str(const llvm::Type &type);
std::string to_str(const Value &value);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
