#include "pool.hpp"

namespace stacksafe {

MapRef::MapRef(const Map& m) : map_{&m} {}

}  // namespace stacksafe
