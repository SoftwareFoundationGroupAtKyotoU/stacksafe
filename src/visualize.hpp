#ifndef INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
#define INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5

#include <llvm/IR/InstVisitor.h>

namespace llvm {
#define HANDLE_INST(NUM, OPCODE, CLASS) class CLASS;
#include <llvm/IR/Instruction.def>
#undef HANDLE_INST
}
namespace stacksafe {
  constexpr auto endl = "\n";

  class ClassNameVisitor :
    public llvm::InstVisitor<ClassNameVisitor, const char *> {
    using RetTy = const char *;
  public:
#define HANDLE_INST(NUM, OPCODE, CLASS) \
    RetTy visit##OPCODE(llvm::CLASS &I) { return #CLASS ": " #OPCODE; }
#include <llvm/IR/Instruction.def>
#undef HANDLE_INST
  };

  template <class T>
  llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const T &x) {
    x.print(O);
    return O;
  }
}

#endif//INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
