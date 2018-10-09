#ifndef INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
#define INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664

#include <functional>

namespace llvm {
  class Value;
  class raw_ostream;
}
namespace stacksafe {
  class Register {
    llvm::Value *reg_;
  public:
    Register(llvm::Value &value);
    std::size_t hash() const;
    void print(llvm::raw_ostream &O) const;
  };
  int get_number(llvm::Value &v);
}
namespace std {
  template <>
  struct hash<stacksafe::Register> {
    size_t operator()(stacksafe::Register r) const;
  };
}

#endif//INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
