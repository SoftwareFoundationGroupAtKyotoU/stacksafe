#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include "json_fwd.hpp"
#include "map.hpp"

namespace stacksafe {

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
};
void to_json(Json& j, const Memory& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
