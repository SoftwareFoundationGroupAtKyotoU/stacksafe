#include "matrix.hpp"
#include <cassert>

namespace stacksafe {

Matrix::Matrix(std::size_t n) : Matrix{n, false} {}
Matrix::Matrix(std::size_t n, bool init)
    : Super(n * n, init ? 1 : 0), size_{n} {}
void Matrix::init(std::size_t n, bool init) {
  size_ = n;
  Super::assign(n * n, init ? 1 : 0);
}
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

}  // namespace stacksafe
