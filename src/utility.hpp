#ifndef INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
#define INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4

#include <string>

namespace llvm {
class CallInst;
class Constant;
class Instruction;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Map;
class Value;

void unsupported_instruction(const llvm::Instruction& i);
bool is_global(const llvm::Constant& c);
bool is_register(const llvm::Instruction& i);
bool is_return(const llvm::CallInst& i);
std::size_t hash_value(const void* ptr);
std::size_t hash_combine(std::size_t lhs, std::size_t rhs);

namespace debug {
int get_operand(const Value& v);
std::string to_str(int num);
std::string to_str(const Domain& dom);
std::string to_str(const Map& map);
}  // namespace debug
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
