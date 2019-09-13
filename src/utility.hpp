#ifndef INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
#define INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B

#include <string>

namespace llvm {
class Type;
class Value;
}  // namespace llvm

namespace stacksafe {

std::string to_str(const llvm::Type& type);
std::string to_str(const llvm::Value& value);
std::string get_operand(const llvm::Value& value);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
