#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <vector>
#include "json_fwd.hpp"
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Symbol;
class Value;
using Params = std::vector<Value>;

class Env {
  Heap heap_;
  Stack stack_;

 public:
  Env() = default;
  explicit Env(const llvm::Function& f);
  const Heap& heap() const;
  const Stack& stack() const;
  bool includes(const Env& that) const;
  void merge(const Env& that);
  void insert_stack(const Value& key, const Domain& val);
  void insert_heap(const Symbol& key, const Domain& val);
  Domain from_stack(const Value& reg) const;
  Domain from_heap(const Symbol& sym) const;
  void call(const Domain& dom);
  Domain collect(const Params& value) const;

 private:
  void collect(const Symbol& symbol, Domain& done) const;
};
void to_json(Json& j, const Env& x);

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

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
