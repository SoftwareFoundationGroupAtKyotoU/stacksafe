#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {

Depend::Depend(std::size_t n) : size_{n} {
  Super::resize(width() * height());
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
void Depend::set_return(const Symbol& sym) {
  auto from = index(sym);
  auto to = size_ + 1;
  set(from, to);
}
bool Depend::is_error() const {
  unsigned from = size_ + 1;
  for (unsigned to = 0; 0 < width(); ++to) {
    if (get(from, to)) {
      return true;
    }
  }
  return false;
}
unsigned Depend::width() const {
  return size_ + 2;
}
unsigned Depend::height() const {
  return size_ + 2;
}
void Depend::set(unsigned from, unsigned to) {
  assert(from < height() && to < width());
  Super::at(width() * from + to) = 1;
}
bool Depend::get(unsigned from, unsigned to) const {
  assert(from < height() && to < width());
  return Super::at(width() * from + to);
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
