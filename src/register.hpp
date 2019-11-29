#ifndef INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954
#define INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954

namespace llvm {
class Argument;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {

class Register {
  const llvm::Value* reg_;

 public:
  explicit Register(const llvm::Instruction& i);
  explicit Register(const llvm::Argument& a);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954
