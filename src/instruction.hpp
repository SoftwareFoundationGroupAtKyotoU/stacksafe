#ifndef INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
#define INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016

#include <vector>

namespace stacksafe {
class Memory;
class Value;

class Params : private std::vector<Value> {
  using Super = std::vector<Value>;

 public:
  using Super::Super, Super::begin, Super::end, Super::push_back;
};

namespace instr {

void binop(Memory& e, const Value& dst, const Value& lhs, const Value& rhs);
void alloc(Memory& e, const Value& dst);
void load(Memory& e, const Value& dst, const Value& src);
void store(Memory& e, const Value& src, const Value& dst);
void cmpxchg(Memory& e, const Value& dst, const Value& ptr, const Value& val);
void cast(Memory& e, const Value& dst, const Value& src);
void phi(Memory& e, const Value& dst, const Params& params);
void call(Memory& e, const Params& params);
void call(Memory& e, const Value& dst, const Params& params);
void constant(Memory& e, const Value& dst);

}  // namespace instr
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D6B0C2A7_2143_420D_BC32_39DCE3F7A016
