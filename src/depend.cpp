#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {

Depend::Depend(std::size_t n) : Super((n + 2) * (n + 2)), size_{n} {
  for (unsigned i = 0; i < n; ++i) {
    set(i, i);
  }
  set(n, n);
}
void Depend::set(const Symbol& key, const Symbol& val) {
  auto to = index(key);
  auto from = index(val);
  if (to < size_ + 1) {
    set(from, to);
  }
}
void Depend::set(unsigned from, unsigned to) {
  assert(from < size_ + 2 && to < size_ + 2);
  Super::at((size_ + 2) * from + to) = 1;
}
unsigned Depend::index(const Symbol& sym) const {
  if (sym.is_local()) {
    return size_ + 1;
  } else if (sym.is_global()) {
    return size_;
  } else {
    auto arg = sym.as_argument();
    assert(arg && "invalid symbol");
    return arg->getArgNo();
  }
}
}  // namespace stacksafe
