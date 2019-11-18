#include "pool.hpp"

namespace stacksafe {

MapRef::MapRef(const Map& m) : map_{&m} {}
const Map& MapRef::get() const {
  return *map_;
}

}  // namespace stacksafe
