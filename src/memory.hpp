#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include "json_fwd.hpp"
#include "map.hpp"
#include "register.hpp"

namespace stacksafe {
class Fabric;

class Memory {
  Heap heap_;
  Stack stack_;

 public:
  const Heap &heap() const;
  const Stack &stack() const;
  bool includes(const Memory &that) const;
  void merge(const Memory &that);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Register &key) const;
  void insert(const Symbol &key, const Domain &val);
  void insert(const Register &key, const Domain &val);
};
void to_json(Json &j, const Memory &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
