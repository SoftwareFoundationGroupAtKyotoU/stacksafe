#include "env.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

template class Env<Register>;
template class Env<Symbol>;

}  // namespace stacksafe
