#ifndef INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
#define INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664

namespace llvm {
  class Value;
}
namespace stacksafe {
  class Register {
    llvm::Value *reg_;
  public:
    Register(llvm::Value &value);
  };
}

#endif//INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
