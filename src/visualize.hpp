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
  struct Manip : std::vector<ManipObj> {
    explicit Manip(std::size_t s = 0);
    Manip(std::initializer_list<ManipObj> l);
    void print(llvm::raw_ostream &O) const;
  };

  const auto wrap = [](const auto &x) -> ManipObj {
    return [=](llvm::raw_ostream &O) { O << x; };
  };
  const auto foreach = [](const auto &f, const auto &c) -> Manip {
    Manip ret(c.size());
    for (const auto &e : c) {
      ret.push_back(f(e));
    }
    return ret;
  };
  const auto manip_single = [](const auto &x) -> Manip {
    return Manip{wrap(x)};
  };
  const auto manip_multi = [](const auto &c) -> Manip {
    return foreach(wrap, c);
  };
  const auto endl = Manip{wrap("\n")};
  Manip parens(const Manip &m);
  Manip braces(const Manip &m);
  Manip squares(const Manip &m);
  Manip angles(const Manip &m);
  Manip join(const ManipObj &sep, const Manip &m);

  template <class T>
  llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const T &x) {
    x.print(O);
    return O;
  }
}

#endif//INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
