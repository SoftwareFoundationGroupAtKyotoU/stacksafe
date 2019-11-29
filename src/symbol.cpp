#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {
namespace {
std::uintptr_t to_symbol(const void* p) {
  static constexpr auto symbol_flag = 0x1;
  if (least_significant_bit(p)) {
    fatal_error("Value representation may conflict");
  }
  if (p) {
    const auto val = reinterpret_cast<std::uintptr_t>(p);
    if (val == 0) {
      fatal_error("nullptr representation may conflict");
    }
    return val & symbol_flag;
  } else {
    return symbol_flag;
  }
}
}  // namespace

Symbol::Symbol(const llvm::AllocaInst& i) : sym_{to_symbol(&i)} {}
Symbol::Symbol(const llvm::Argument& a) : sym_{to_symbol(&a)} {}

}  // namespace stacksafe
