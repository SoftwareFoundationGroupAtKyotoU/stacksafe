#include "matrix.hpp"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <optional>

namespace stacksafe {
namespace {
std::optional<int> to_int(std::string_view v) {
  const std::string buf{v};
  const auto b = buf.c_str();
  const auto e = b + buf.size();
  if (b != e) {
    char* p = nullptr;
    const auto val = std::strtol(b, &p, 10);
    if (p == e && 0 <= val && val != LONG_MAX && val <= INT_MAX) {
      return static_cast<int>(val);
    }
  }
  return std::nullopt;
}
enum class IndexInit : int { GLOBAL = -1, RETURN = -2, OTHERS = -3 };
constexpr std::size_t extra_space = 2;
}  // namespace

const Index Index::GLOBAL{IndexInit::GLOBAL};
const Index Index::RETURN{IndexInit::RETURN};
const Index Index::OTHERS{IndexInit::OTHERS};
Index::Index(IndexInit init) : index_{static_cast<int>(init)} {}
Index::Index(int index) : index_{index} {
  assert(0 <= index);
}
bool Index::is_valid(Index arity) const {
  return OTHERS.index_ < index_ && index_ < arity.index_;
}
Index::operator int() const {
  return index_;
}
Index::operator bool() const {
  return index_ != OTHERS.index_;
}
Arity::Arity(int arity) : Index{arity} {
  assert(0 <= arity);
}
std::size_t Arity::value() const {
  return index_;
}
Index Arity::index(int i) const {
  if (OTHERS.index_ < i && i < this->index_) {
    return Index{i};
  }
  return Index::OTHERS;
}
Index Arity::to_index(std::string_view v) const {
  if (v == "g") {
    return GLOBAL;
  } else if (v == "r") {
    return RETURN;
  } else if (const auto i = to_int(v);
             i && OTHERS.index_ < *i && *i < this->index_) {
    return Index{*i};
  } else {
    return OTHERS;
  }
}

Matrix::Matrix(std::size_t n) : Matrix{n, false} {}
Matrix::Matrix(Arity arity) : Matrix{arity.value() + extra_space, false} {}
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
