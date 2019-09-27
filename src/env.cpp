#include "env.hpp"

namespace stacksafe {

Env::Env(Memory& m, RegisterCache& r) : mem_{m}, cache_{r} {}

}  // namespace stacksafe
