#ifndef INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
#define INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace llvm {
class raw_ostream;
}

namespace stacksafe {
class Symbol;

class Matrix : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;
  std::size_t size_;

 public:
  explicit Matrix(std::size_t n);
  std::size_t size() const;
  void set(std::size_t row, std::size_t col);
  bool get(std::size_t row, std::size_t col) const;
};

class Depend : private Matrix {
 public:
  explicit Depend(std::size_t n);
  void init(std::string_view v);
  void set(std::string_view pair);
  void set(const Symbol& key, const Symbol& val);
  void set_return(const Symbol& sym);
  bool is_error() const;
  void print(llvm::raw_ostream& os) const;
  void print_error(llvm::raw_ostream& os) const;

 private:
  std::size_t local_index() const;
  std::size_t global_index() const;
  void set(std::size_t from, std::size_t to);
  bool get(std::size_t from, std::size_t to) const;
  bool get_error(std::size_t to) const;
  std::size_t to_index(const Symbol& sym) const;
  std::size_t to_index(std::string_view v) const;
  std::string to_str(std::size_t i) const;
};

class DependMap : private std::map<std::string, Depend> {
  using Super = std::map<std::string, Depend>;

 public:
  DependMap();

 private:
  Depend* init(std::string_view header);
  void load(std::string_view line);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
