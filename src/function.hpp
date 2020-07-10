#ifndef INCLUDE_GUARD_6584D3A4_3AC2_4F36_AE36_A47C2A982896
#define INCLUDE_GUARD_6584D3A4_3AC2_4F36_AE36_A47C2A982896

#include <vector>
#include "tarjan.hpp"

namespace llvm {
class Function;
class Module;
}  // namespace llvm

namespace dataflow {
namespace function {
using Component = std::vector<const llvm::Function *>;
class Scc : public Tarjan {
 public:
  static std::vector<Component> solve(const llvm::Module &m);

 private:
  Vec successors(Ptr f) const override;
};
}  // namespace function
}  // namespace dataflow

#endif  // INCLUDE_GUARD_6584D3A4_3AC2_4F36_AE36_A47C2A982896
