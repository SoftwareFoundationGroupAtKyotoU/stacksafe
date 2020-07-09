#ifndef INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77
#define INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77

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
  const llvm::Value* value() const;
  int level() const;
};
}  // namespace dataflow

#endif  // INCLUDE_GUARD_0B4B9978_D346_4DE5_B632_5DCD571D3B77
