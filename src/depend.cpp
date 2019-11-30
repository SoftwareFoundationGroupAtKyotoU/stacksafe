#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/Support/raw_ostream.h>
#include <cassert>
#include "symbol.hpp"

namespace stacksafe {

Matrix::Matrix(std::size_t n) : Super(n * n, 0), size_{n} {}
std::size_t Matrix::size() const {
  return size_;
}
void Matrix::set(std::size_t row, std::size_t col) {
  assert(row < size_ && col < size_);
  Super::at(row * size_ + col) = 1;
}
bool Matrix::get(std::size_t row, std::size_t col) const {
  assert(row < size_ && col < size_);
  return Super::at(row * size_ + col) != 0;
}

Depend::Depend(std::size_t n) : Matrix{n + 2} {}
void Depend::set(const Symbol& key, const Symbol& val) {
  const auto to = index(key);
  const auto from = index(val);
  if (from != to && to < local_index()) {
    Matrix::set(from, to);
  }
}
void Depend::set_return(const Symbol& sym) {
  const auto from = index(sym);
  const auto to = local_index();
  Matrix::set(from, to);
}
bool Depend::is_error() const {
  return is_error_argument() || is_error_global() || is_error_return();
}
void Depend::print(llvm::raw_ostream& os) const {
  static const auto comma = ",";
  static const auto colon = ":";
  for (std::size_t from = 0; from < local_index(); ++from) {
    for (std::size_t to = 0; to < Matrix::size(); ++to) {
      if (get(from, to)) {
        os << comma << to_str(from) << colon << to_str(to);
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
std::size_t Depend::local_index() const {
  return Matrix::size() - 1;
}
std::size_t Depend::global_index() const {
  return Matrix::size() - 2;
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
bool Depend::get(std::size_t from, std::size_t to) const {
  return !diagonal(from, to) && Matrix::get(from, to);
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
