#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include "blocks.hpp"
#include "log.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  Log log_;
  Blocks blocks_;
  const llvm::Function &func_;
  bool safe_;

 public:
  explicit Abstract(const llvm::Function &f);
  void run();
  void print(llvm::raw_ostream &os) const;

 private:
  void interpret(const llvm::BasicBlock &b);
  bool is_safe() const;
  void set_unsafe();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
