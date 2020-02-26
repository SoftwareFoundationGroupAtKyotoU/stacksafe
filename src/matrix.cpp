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
Index::Index(int index) : index_{index} {
  assert(GLOBAL < *this);
}
Index::Index(IndexInit init) : index_{static_cast<int>(init)} {}
Index::operator bool() const {
  return !(*this == OTHERS);
}
bool Index::operator==(const Index& that) const {
  return this->index_ == that.index_;
}
bool Index::operator<(const Index& that) const {
  return this->index_ < that.index_;
}

Arity::Arity(int arity) : Index{arity} {
  assert(0 <= arity);
}
std::size_t Arity::value() const {
  return index_;
}
std::size_t Arity::convert(Index index) const {
  assert(is_valid(index));
  return this->index_ - index.index_ - 1;
}
std::vector<Index> Arity::indices() const {
  std::vector<Index> vec{GLOBAL};
  for (int i = 0; i < index_; ++i) {
    vec.push_back(Index{i});
  }
  return vec;
}
bool Arity::is_valid(Index index) const {
  return OTHERS < index && index < *this;
}
Index Arity::to_index(std::string_view v) const {
  if (v == "g") {
    return GLOBAL;
  } else if (v == "r") {
    return RETURN;
  } else if (const auto i = to_int(v); i && *i < index_) {
    return Index{*i};
  } else {
    return OTHERS;
  }
}
Index Arity::to_index(std::size_t i) const {
  const int index = i;
  if (index < index_) {
    return Index{index};
  }
  return OTHERS;
}

Matrix::Matrix(std::size_t n) : Matrix{n, false} {}
Matrix::Matrix(const Arity& arity)
    : Matrix{arity.value() + extra_space, false} {}
Matrix::Matrix(std::size_t n, bool init)
    : Super(n * n, init ? 1 : 0),
      size_{n},
      arity_{static_cast<int>(n) - static_cast<int>(extra_space)} {}
void Matrix::init(std::size_t n, bool init) {
  size_ = n;
  Super::assign(n * n, init ? 1 : 0);
}
const Arity& Matrix::arity() const {
  return arity_;
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
void Matrix::set(Index row, Index col) {
  set(arity_.convert(row), arity_.convert(col));
}
bool Matrix::get(Index row, Index col) const {
  return get(arity_.convert(row), arity_.convert(col));
}

}  // namespace stacksafe
