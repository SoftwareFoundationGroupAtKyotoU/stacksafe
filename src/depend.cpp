#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <cassert>
#include <optional>
#include "symbol.hpp"

namespace stacksafe {
namespace {
std::optional<unsigned> to_number(const Symbol& sym, unsigned size) {
  if (sym.is_global()) {
    return size;
  } else if (auto arg = sym.as_argument()) {
    return arg->getArgNo();
  } else {
    return std::nullopt;
  }
}
}  // namespace

Depend::Depend(unsigned n) : Super((n + 2) * (n + 1)), size_{n} {
  for (unsigned i = 0; i < n; ++i) {
    set(i, i);
  }
  set(n, n);
}
bool Depend::set(const Symbol& from, const Symbol& to) {
  if (auto col = to_number(to, size_)) {
    if (auto row = to_number(from, size_)) {
      set(*row, *col);
    } else {
      return false;
    }
  }
  return true;
}
void Depend::set(unsigned from, unsigned to) {
  assert(from < size_ + 1 && to < size_ + 2);
  Super::at((size_ + 2) * from + to) = 1;
}

}  // namespace stacksafe
