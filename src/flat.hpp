#ifndef INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
#define INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8

#include "symbol.hpp"
#include "value.hpp"

namespace stacksafe {

class MapsTo : private std::tuple<Value, Symbol> {
  using Super = std::tuple<Value, Symbol>;

 public:
  MapsTo(const Value& val, const Symbol& sym);
  const Value& value() const;
  const Symbol& symbol() const;
  static std::size_t hash(const MapsTo& to);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
