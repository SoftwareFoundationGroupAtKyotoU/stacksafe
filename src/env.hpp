#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include "map.hpp"

namespace stacksafe {
class Value;

class Env {
  Stack stack_;
  Heap heap_;

 public:
  void alloc(const Value& dst);
  void store(const Value& src, const Value& dst);
  void load(const Value& dst, const Value& src);
  void show() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
