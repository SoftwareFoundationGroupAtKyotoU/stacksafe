#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/Support/raw_ostream.h>
#include <cassert>
#include "symbol.hpp"
#include "utility.hpp"

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
  const auto to = to_index(key);
  const auto from = to_index(val);
  if (from != to && to < local_index()) {
    Matrix::set(from, to);
  }
}
void Depend::set_return(const Symbol& sym) {
  const auto from = to_index(sym);
  const auto to = local_index();
  Matrix::set(from, to);
}
bool Depend::is_error() const {
  for (auto to = 0_z; to < Matrix::size(); ++to) {
    if (get_error(to)) {
      return true;
    }
  }
  return false;
}
void Depend::print(llvm::raw_ostream& os) const {
  static const auto comma = ",";
  static const auto colon = ":";
  if (is_error()) {
    return;
  }
  for (auto from = 0_z; from < Matrix::size(); ++from) {
    for (auto to = 0_z; to < Matrix::size(); ++to) {
      if (get(from, to)) {
        assert(from != local_index());
        os << comma << to_str(from) << colon << to_str(to);
      }
    }
  }
}
void Depend::print_error(llvm::raw_ostream& os) const {
  for (auto to = 0_z; to < global_index(); ++to) {
    if (get_error(to)) {
      os << "ERROR: ARGUMENT\n";
      break;
    }
  }
  if (get_error(global_index())) {
    os << "ERROR: GLOBAL\n";
  }
  if (get_error(local_index())) {
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
bool Depend::diagonal(std::size_t from, std::size_t to) const {
  return from == to && from < local_index();
}
bool Depend::get(std::size_t from, std::size_t to) const {
  return !diagonal(from, to) && Matrix::get(from, to);
}
bool Depend::get_error(std::size_t to) const {
  return Matrix::get(local_index(), to);
}
std::size_t Depend::to_index(const Symbol& sym) const {
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
