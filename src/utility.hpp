#ifndef INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
#define INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B

#include <llvm/Support/ErrorHandling.h>
#include <optional>
#include <string>
#include <string_view>

namespace llvm {
class Instruction;
class Type;
class Value;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Value;
enum class ValueKind;

#define stacksafe_unreachable(msg, obj) \
  llvm_unreachable((std::string{"Error: " msg ": "} + to_str(obj)).c_str())

std::optional<int> to_int(std::string_view view);
std::string to_alphabet(int num);
std::string to_str(const llvm::Type& type);
std::string to_str(const llvm::Value& value);
std::string to_str(const Value& value);
std::string get_operand(const llvm::Value& value);

void endline(llvm::raw_ostream& os);
void print_string(llvm::raw_ostream& os, const std::string& str);
void print_stdout(const std::string& str);
bool is_voidfunc(const llvm::Value& v);
bool is_register(const llvm::Value& v);
bool is_global(const llvm::Value& v);
bool is_constant(const llvm::Value& v);
bool check_register(const llvm::Value& v);
void unknown_instruction(const llvm::Instruction& i);
void check_interpret(const llvm::Instruction& i, bool success);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
