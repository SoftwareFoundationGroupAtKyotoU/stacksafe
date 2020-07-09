#ifndef INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
#define INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4

#include <string>

namespace llvm {
class CallInst;
class Value;
}  // namespace llvm

namespace stacksafe {

std::size_t operator"" _z(unsigned long long n);
std::string to_str(const llvm::Value &v);
[[noreturn]] void fatal_error(const std::string &msg);
bool least_significant_bit(const void *ptr);
bool is_global(const llvm::Value &v);
bool is_register(const llvm::Value &v);
bool is_return(const llvm::CallInst &i);

namespace debug {
int get_operand(const llvm::Value *v);
void print(const std::string &str);
std::string to_str(int num);
}  // namespace debug
}  // namespace stacksafe

namespace dataflow {
namespace debug {
void print(const std::string &str);
std::string to_str(const llvm::Value &v);
std::string to_label(const llvm::Value *v);
}  // namespace debug
}  // namespace dataflow

#endif  // INCLUDE_GUARD_52E9C5D0_9006_4FB1_8F70_5B1B891C7DA4
