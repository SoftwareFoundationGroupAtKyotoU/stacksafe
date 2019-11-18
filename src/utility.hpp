#ifndef INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
#define INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4

#include <string>

namespace llvm {
class Constant;
class Instruction;
}  // namespace llvm

namespace stacksafe {
class Value;

void unsupported_instruction(const llvm::Instruction& i);
bool is_argument(const Value& v);
bool is_constant(const Value& v);
bool is_global(const llvm::Constant& c);
bool is_register(const llvm::Instruction& i);
bool is_register(const Value& v);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
