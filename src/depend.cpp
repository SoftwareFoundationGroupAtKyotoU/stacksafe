#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/Support/raw_ostream.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {

Depend::Depend(std::size_t n) : size_{n} {
  Super::resize(width() * height());
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
  return is_error_argument() || is_error_global() || is_error_return();
}
bool Depend::is_empty() const {
  for (std::size_t from = 0; from < local_index(); ++from) {
    for (std::size_t to = 0; to < width(); ++to) {
      if (get(from, to)) {
        return false;
      }
    }
  }
  return true;
}
void Depend::print(llvm::raw_ostream& os) const {
  static const auto comma = ",";
  static const auto colon = ":";
  for (std::size_t from = 0; from < local_index(); ++from) {
    for (std::size_t to = 0; to < local_index(); ++to) {
      if (get(from, to) && from != to) {
        os << comma << from << colon << to;
      }
    }
  }
}
void Depend::print_error(llvm::raw_ostream& os) const {
  if (is_error_argument()) {
    os << "ERROR: ARGUMENT\n";
  }
  if (is_error_global()) {
    os << "ERROR: GLOBAL\n";
  }
  if (is_error_return()) {
    os << "ERROR: RETURN\n";
  }
  os.flush();
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
std::size_t Depend::global_index() const {
  return size_;
}
bool Depend::is_error_argument() const {
  auto from = local_index();
  for (std::size_t to = 0; to < global_index(); ++to) {
    if (get(from, to)) {
      return true;
    }
  }
  return false;
}
bool Depend::is_error_global() const {
  return get(local_index(), global_index());
}
bool Depend::is_error_return() const {
  return get(local_index(), local_index());
}
bool Depend::diagonal(std::size_t from, std::size_t to) const {
  return from == to && from < local_index();
}
void Depend::set(std::size_t from, std::size_t to) {
  assert(from < height() && to < width());
  Super::at(width() * from + to) = (diagonal(from, to) ? 0 : 1);
}
bool Depend::get(std::size_t from, std::size_t to) const {
  assert(from < height() && to < width());
  return !diagonal(from, to) && Super::at(width() * from + to);
}
std::size_t Depend::index(const Symbol& sym) const {
  if (sym.is_local()) {
    return local_index();
  } else if (sym.is_global()) {
    return global_index();
  } else {
    auto arg = sym.as_argument();
    assert(arg && "invalid symbol");
    return arg->getArgNo();
  }
}
std::string Depend::to_str(std::size_t i) const {
  if (local_index() == i) {
    return "r";
  } else if (global_index() == i) {
    return "g";
  } else {
    return std::to_string(i);
  }
}
}  // namespace stacksafe
