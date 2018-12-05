#ifndef INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
#define INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664

#include <functional>
#include <optional>

namespace llvm {
  class Value;
  class raw_ostream;
}
namespace stacksafe {
  class Value {
    llvm::Value *val_;
    int num_;
  public:
    explicit Value(llvm::Value &v);
    const llvm::Value &get() const;
    bool is_register() const;
    void print(llvm::raw_ostream &O) const;
  };

  class Register {
    llvm::Value *reg_;
    int num_;
    Register(llvm::Value &v, int n);
    friend std::optional<Register> make_register(llvm::Value &);
  public:
    const llvm::Value &get() const;
    std::size_t hash() const;
    bool operator==(const Register &rhs) const;
    void print(llvm::raw_ostream &O) const;
  };
  std::optional<Register> make_register(llvm::Value &v);

  int get_number(llvm::Value &v);
}
namespace std {
  template <>
  struct hash<stacksafe::Register> {
    size_t operator()(stacksafe::Register r) const;
  };
}

#endif//INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664
