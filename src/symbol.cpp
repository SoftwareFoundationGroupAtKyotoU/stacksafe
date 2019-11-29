#include "symbol.hpp"
#include <cassert>
#include "utility.hpp"

namespace stacksafe {
namespace {
constexpr std::uintptr_t symbol_flag = 0x1;
std::uintptr_t to_symbol(const void* p) {
  assert(p);
  const auto val = reinterpret_cast<std::uintptr_t>(p);
  if (val == 0) {
    fatal_error("nullptr representation may conflict");
  }
  return val & symbol_flag;
}
}  // namespace

Symbol::Symbol(const void* p) : sym_{to_symbol(p)} {
  if (least_significant_bit(p)) {
    fatal_error("Value representation may conflict");
  }
}
Symbol::Symbol() : sym_{symbol_flag} {}
Symbol::Symbol(const llvm::AllocaInst& i) : Symbol{&i} {}
Symbol::Symbol(const llvm::Argument& a) : Symbol{&a} {}

}  // namespace stacksafe
