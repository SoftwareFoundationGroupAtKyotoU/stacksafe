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
  friend class Value;
  const llvm::Value* value_;
  int level_;
  Cell(const llvm::Value* value, int level);
  static Cell make(const llvm::Value* value);

 public:
  static Cell shift(const Cell& cell, int level);
  const llvm::Value* value() const;
  int level() const;
  std::string to_string() const;
  bool is_local() const;
};
bool operator<(const Cell& lhs, const Cell& rhs);
void to_json(nlohmann::json& j, const Cell& cell);

class Value : private std::set<Cell> {
  using Super = std::set<Cell>;

 public:
  using Super::begin, Super::end, Super::insert;
  Value();
  static Value make(const llvm::Value* v);
  void insert(const Value& value);
  bool is_local() const;
  bool is_global() const;
};
void to_json(nlohmann::json& j, const Value& value);
}  // namespace dataflow

#endif  // INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77
