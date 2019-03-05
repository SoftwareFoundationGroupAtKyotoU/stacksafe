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
  protected:
    const llvm::Value *val_;
    int num_;
  public:
    explicit Value(const llvm::Value &v);
    const llvm::Value &get() const;
    std::size_t hash() const;
    bool operator==(const Value &rhs) const;
    bool is_register() const;
    void print(llvm::raw_ostream &O) const;
  };

  class Register : public Value {
  public:
    explicit Register(const llvm::Value &v);
  };
  std::optional<Register> make_register(const llvm::Value &v);

  int get_number(const llvm::Value &v);
}
namespace std {
  template <>
  struct hash<stacksafe::Value> {
    size_t operator()(const stacksafe::Value &v) const;
  };
  template <>
  struct hash<stacksafe::Register> {
    size_t operator()(const stacksafe::Register &r) const;
  };
}

#endif//INCLUDE_GUARD_40FBF2CB_F11A_46FD_8C86_B65C80D3F664