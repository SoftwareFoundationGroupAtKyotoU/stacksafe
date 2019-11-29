#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {
namespace {
unsigned to_number(const Symbol& sym, unsigned size) {
  if (sym.is_global()) {
    return size;
  } else if (auto arg = sym.as_argument()) {
    return arg->getArgNo();
  } else {
    return size + 1;
  }
}
}  // namespace

Depend::Depend(std::size_t n) : Super((n + 2) * (n + 2)), size_{n} {
  for (unsigned i = 0; i < n; ++i) {
    set(i, i);
  }
  set(n, n);
}
void Depend::set(const Symbol& key, const Symbol& val) {
  auto to = to_number(key, size_);
  auto from = to_number(val, size_);
  if (to < size_ + 1) {
    set(from, to);
  }
}
void Depend::set(unsigned from, unsigned to) {
  assert(from < size_ + 2 && to < size_ + 2);
  Super::at((size_ + 2) * from + to) = 1;
}

}  // namespace stacksafe
