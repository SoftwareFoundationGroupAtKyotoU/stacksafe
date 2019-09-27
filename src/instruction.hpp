#ifndef INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
#define INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016

#include <set>
#include <vector>

namespace llvm {
class Value;
}

namespace stacksafe {
class Env;
class Memory;
class Value;

using ValueSet = std::set<const llvm::Value*>;

namespace instr {

void binop(Env& e, const llvm::Value& dst, const llvm::Value& lhs,
           const llvm::Value& rhs);
void alloc(Env& e, const llvm::Value& dst);
void load(Env& e, const llvm::Value& dst, const llvm::Value& src);
void store(Env& e, const llvm::Value& src, const llvm::Value& dst);
void cmpxchg(Env& e, const llvm::Value& dst, const llvm::Value& ptr,
             const llvm::Value& val);
void cast(Env& e, const llvm::Value& dst, const llvm::Value& src);
void phi(Env& e, const llvm::Value& dst, const ValueSet& params);
void call(Env& e, const ValueSet& params);
void call(Env& e, const llvm::Value& dst, const ValueSet& params);
void constant(Env& e, const llvm::Value& dst);

}  // namespace instr
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
