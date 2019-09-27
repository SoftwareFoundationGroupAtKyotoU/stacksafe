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

class Memory {
  Heap heap_;
  RegMap regmap_;

 public:
  Memory() = default;
  const Heap& heap() const;
  Heap& heap();
  const RegMap& stack() const;
  RegMap& stack();
  bool includes(const Memory& that) const;
  void merge(const Memory& that);
  void insert_stack(const Register& key, const Domain& val);
  void insert_heap(const Symbol& key, const Domain& val);
  Domain from_stack(const Register& key) const;
  Domain from_heap(const Symbol& sym) const;
};
void to_json(Json& j, const Memory& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
