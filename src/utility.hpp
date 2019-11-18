#ifndef INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
#define INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4

#include <llvm/Support/ErrorHandling.h>
#include <string>

#define stacksafe_unreachable(msg, val) \
  llvm_unreachable(                     \
      (std::string{"UNREACHABLE: " msg ": "} + to_str(val)).c_str())

namespace llvm {
class Constant;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {
class Value;

std::string to_str(const Value& v);
std::string get_operand(const Value& v);
bool is_argument(const llvm::Value& v);
bool is_global(const llvm::Constant& c);
bool is_register(const llvm::Instruction& i);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
