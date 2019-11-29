#include "symbol.hpp"
#include <llvm/IR/Argument.h>
#include "utility.hpp"

namespace stacksafe {
namespace {
constexpr std::uintptr_t symbol_flag = 0x1;
std::uintptr_t to_symbol(const void* p) {
  if (p) {
    const auto val = reinterpret_cast<std::uintptr_t>(p);
    if (val == 0) {
      fatal_error("nullptr representation may conflict");
    }
    return val | symbol_flag;
  } else {
    return symbol_flag;
  }
}
const llvm::Value* to_pointer(std::uintptr_t u) {
  const auto p = u & ~symbol_flag;
  if (p) {
    return reinterpret_cast<const llvm::Value*>(p);
  } else {
    return nullptr;
  }
}
}  // namespace

Symbol::Symbol(const void* p) : sym_{to_symbol(p)} {
  if (least_significant_bit(p)) {
    fatal_error("Value representation may conflict");
  }
}
Symbol::Symbol() : sym_{to_symbol(nullptr)} {}
Symbol::Symbol(const llvm::AllocaInst& i) : Symbol{&i} {}
Symbol::Symbol(const llvm::Argument& a) : Symbol{&a} {}
bool Symbol::is_global() const {
  return to_pointer(sym_) == nullptr;
}
const llvm::Argument* Symbol::as_argument() const {
  return llvm::dyn_cast_or_null<llvm::Argument>(to_pointer(sym_));
}

}  // namespace stacksafe
