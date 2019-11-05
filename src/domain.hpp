#ifndef INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
#define INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0

#include <unordered_set>
#include "json_fwd.hpp"
#include "register.hpp"

namespace stacksafe {

class Domain : private std::unordered_set<Register> {
  using Super = std::unordered_set<Register>;

 public:
  using Super::begin, Super::end, Super::empty, Super::size;
  Domain() = default;
  explicit Domain(std::initializer_list<Register> list);
  void merge(const Domain &that);
  bool includes(const Domain &that) const;
  Domain minus(const Domain &that) const;
  bool has_local() const;
  static const Domain &get_empty();
  static const Domain &get_global();
};
void to_json(Json &j, const Domain &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B43C0EC1_4338_47A3_AD6C_40B0DEF734B0
