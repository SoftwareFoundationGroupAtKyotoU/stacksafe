#ifndef INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954
#define INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954

namespace llvm {
class Value;
}

namespace stacksafe {

class Register {
  const llvm::Value* reg_;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E89E0FBA_AE75_4472_9027_FDC32CA21954
