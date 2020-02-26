#ifndef INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F
#define INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F

#include <cstdint>
#include <string_view>
#include <vector>

namespace stacksafe {
namespace {
enum class IndexInit;
}

class Index {
  friend class Arity;
  int index_;
  explicit Index(int index);

 public:
  static const Index GLOBAL, RETURN, OTHERS;
  explicit Index(IndexInit init);
  Index& operator++();
  explicit operator bool() const;
  bool operator==(const Index& that) const;
  bool operator<(const Index& that) const;
};

class Arity : private Index {
 public:
  explicit Arity(int arity);
  std::size_t value() const;
  std::size_t convert(Index index) const;
  std::vector<Index> indices() const;
  bool is_valid(Index index) const;
  Index to_index(std::string_view v) const;
};

class Matrix : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;
  std::size_t size_;
  Arity arity_;

 public:
  explicit Matrix(std::size_t n);
  explicit Matrix(const Arity& arity);
  Matrix(std::size_t n, bool init);
  void init(std::size_t n, bool init);
  void init(const Arity& arity);
  const Arity& arity() const;
  std::size_t size() const;
  void set(std::size_t row, std::size_t col);
  bool get(std::size_t row, std::size_t col) const;
  void set(Index row, Index col);
  bool get(Index row, Index col) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F
