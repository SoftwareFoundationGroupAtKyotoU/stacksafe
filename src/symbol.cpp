#include "symbol.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instructions.h>

namespace stacksafe {

Symbol::Symbol(const llvm::Value* p) : ptr_{p} {}
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
