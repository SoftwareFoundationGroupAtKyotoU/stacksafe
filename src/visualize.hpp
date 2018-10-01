#ifndef INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
#define INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5

#include <functional>
#include <initializer_list>
#include <vector>
#include <llvm/IR/InstVisitor.h>
#include <llvm/Support/raw_ostream.h>

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

  using ManipObj = std::function<void(llvm::raw_ostream &)>;
  const auto wrap = [](const auto &x) -> ManipObj {
    return [=](llvm::raw_ostream &O) { O << x; };
  };
  struct Manip : std::vector<ManipObj> {
    Manip(std::initializer_list<ManipObj> l);
    void print(llvm::raw_ostream &O) const;
  };
  template <class T>
  llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const T &x) {
    x.print(O);
    return O;
  }
}

#endif//INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
