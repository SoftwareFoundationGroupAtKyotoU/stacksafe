#include "depend.hpp"

namespace stacksafe {

Depend::Depend(unsigned n) : Super((n + 2) * n), size_{n} {}

}  // namespace stacksafe
