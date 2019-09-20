#ifndef INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
#define INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016

#include <vector>

namespace stacksafe {
class Env;
class Value;
using Params = std::vector<Value>;

namespace instr {

void binop(Env& e, const Value& dst, const Value& lhs, const Value& rhs);
void alloc(Env& e, const Value& dst);
void load(Env& e, const Value& dst, const Value& src);
void store(Env& e, const Value& src, const Value& dst);
void cmpxchg(Env& e, const Value& dst, const Value& ptr, const Value& val);
void cast(Env& e, const Value& dst, const Value& src);
void phi(Env& e, const Value& dst, const Params& params);
void call(Env& e, const Params& params);
void call(Env& e, const Value& dst, const Params& params);
void constant(Env& e, const Value& dst);

}  // namespace instr
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
