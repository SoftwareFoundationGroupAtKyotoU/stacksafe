#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {

Depend::Depend(std::size_t n) : size_{n} {
  Super::resize(width() * height());
  for (std::size_t i = 0; i < local_index(); ++i) {
    set(i, i);
  }
}
void Depend::set(const Symbol& key, const Symbol& val) {
  auto to = index(key);
  auto from = index(val);
  if (to < local_index()) {
    set(from, to);
  }
}
void Depend::set_return(const Symbol& sym) {
  auto from = index(sym);
  auto to = local_index();
  set(from, to);
}
bool Depend::is_error() const {
  auto from = local_index();
  for (std::size_t to = 0; 0 < width(); ++to) {
    if (get(from, to)) {
      return true;
    }
  }
  return false;
}
std::size_t Depend::width() const {
  return size_ + 2;
}
std::size_t Depend::height() const {
  return size_ + 2;
}
std::size_t Depend::local_index() const {
  return size_ + 1;
}
void Depend::set(std::size_t from, std::size_t to) {
  assert(from < height() && to < width());
  Super::at(width() * from + to) = 1;
}
bool Depend::get(std::size_t from, std::size_t to) const {
  assert(from < height() && to < width());
  return Super::at(width() * from + to);
}
std::size_t Depend::index(const Symbol& sym) const {
  if (sym.is_local()) {
    return local_index();
  } else if (sym.is_global()) {
    return size_;
  } else {
    auto arg = sym.as_argument();
    assert(arg && "invalid symbol");
    return arg->getArgNo();
  }
}
}  // namespace stacksafe
