#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <string>
#include <unordered_map>
#include "env.hpp"
#include "error.hpp"
#include "log.hpp"
#include "pool.hpp"

namespace llvm {
class BasicBlock;
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  const llvm::Function &func_;
  const Log log_;
  Error error_;
  double elapsed_;

 public:
  explicit Abstract(const llvm::Function &f);
  void run_scc(const llvm::Function &f);
  void print(llvm::raw_ostream &os) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
