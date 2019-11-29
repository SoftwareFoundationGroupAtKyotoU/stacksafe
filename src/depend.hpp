#ifndef INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
#define INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA

#include <cstdint>
#include <vector>

namespace llvm {
class raw_ostream;
}

namespace stacksafe {
class Symbol;

class Depend : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;
  std::size_t size_;

 public:
  explicit Depend(std::size_t n);
  void set(const Symbol& key, const Symbol& val);
  void set_return(const Symbol& sym);
  bool is_error() const;
  void print_error(llvm::raw_ostream& os) const;

 private:
  std::size_t width() const;
  std::size_t height() const;
  std::size_t local_index() const;
  std::size_t global_index() const;
  bool is_error_argument() const;
  bool is_error_global() const;
  bool is_error_return() const;
  void set(std::size_t from, std::size_t to);
  bool get(std::size_t from, std::size_t to) const;
  std::size_t index(const Symbol& sym) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
