#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <string>
#include <unordered_map>
#include "blocks.hpp"
#include "error.hpp"
#include "log.hpp"
#include "pool.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  const Log log_;
  Error error_;
  FlatMapPool pool_;
  std::unordered_map<const llvm::BasicBlock *, Env> blocks_;
  std::string name_;
  double elapsed_;

 public:
  explicit Abstract(const llvm::Function &f);
  void run(const llvm::Function &f);
  void print(llvm::raw_ostream &os) const;

 private:
  void interpret(const llvm::BasicBlock &b);
  Env &get(const llvm::BasicBlock &b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
