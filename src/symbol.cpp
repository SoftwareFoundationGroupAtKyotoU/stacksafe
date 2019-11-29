#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol(const llvm::AllocaInst& i)
    : sym_{reinterpret_cast<std::uintptr_t>(&i) | 0x1} {
  if (least_significant_bit(&i)) {
    fatal_error("Value representation may conflict");
  }
}

}  // namespace stacksafe
