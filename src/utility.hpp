#ifndef INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
#define INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B

#include <optional>
#include <string>
#include <string_view>

namespace llvm {
class Instruction;
class Type;
class Value;
}  // namespace llvm

namespace stacksafe {

std::optional<int> to_int(std::string_view view);
std::string to_alphabet(int num);
std::string to_str(const llvm::Type& type);
std::string to_str(const llvm::Value& value);
std::string get_operand(const llvm::Value& value);

void constant_info(const llvm::Value& v);
void unknown_instruction(const llvm::Instruction& i);
void check_interpret(const llvm::Instruction& i, bool success);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
