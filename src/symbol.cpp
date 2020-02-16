#include "symbol.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instructions.h>
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
}  // namespace

Symbol::Symbol(const llvm::Value* p) : sym_{to_symbol(p)}, ptr_{p} {
  if (p && least_significant_bit(p)) {
    fatal_error("Value representation may conflict");
  }
}
Symbol::Symbol(const llvm::AllocaInst& i) : Symbol{&i} {}
Symbol::Symbol(const llvm::Argument& a) : Symbol{&a} {}
const llvm::Value* Symbol::value() const {
  return ptr_;
}
bool Symbol::is_global() const {
  return !value();
}
bool Symbol::is_local() const {
  return llvm::isa_and_nonnull<llvm::AllocaInst>(value());
}
const llvm::Argument* Symbol::as_argument() const {
  return llvm::dyn_cast_or_null<llvm::Argument>(value());
}
Symbol Symbol::get_global() {
  return Symbol{nullptr};
}

}  // namespace stacksafe
