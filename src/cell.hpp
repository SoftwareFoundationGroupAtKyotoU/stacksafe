#ifndef INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77
#define INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77

#include <set>
#include <string>
#include "nlohmann/json.hpp"

namespace llvm {
class Value;
}

namespace dataflow {
class Cell {
  const llvm::Value* value_;
  int level_;
  Cell(const llvm::Value* value, int level);

 public:
  static Cell make(const llvm::Value* value);
  static Cell deref(const Cell& cell);
  const llvm::Value* value() const;
  int level() const;
  std::string to_string() const;
};
bool operator<(const Cell& lhs, const Cell& rhs);
void to_json(nlohmann::json& j, const Cell& cell);

class Value : private std::set<Cell> {
  using Super = std::set<Cell>;

 public:
  using Super::begin, Super::end;
  Value();
  explicit Value(const llvm::Value* v);
  void insert(const Value& value);

 private:
  void insert(const llvm::Value* value);
};
void to_join(nlohmann::json& j, const Value& set);
}  // namespace dataflow

#endif  // INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77
