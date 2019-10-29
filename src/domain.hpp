#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <set>
#include "json_fwd.hpp"
#include "symbol.hpp"

namespace stacksafe {
class Fabric;

class Domain : private std::set<Symbol> {
  using Super = std::set<Symbol>;

 public:
  using Super::begin, Super::end, Super::empty, Super::size;
  Domain() = default;
  explicit Domain(std::initializer_list<Symbol> list);
  bool merge(const Domain &that);
  bool includes(const Domain &that) const;
  bool has_local() const;
  static Domain global();
};
void to_json(Json &j, const Domain &x);
Fabric dump(const Domain &domain);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
