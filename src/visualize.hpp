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
      ret.push_back(wrap(f(e)));
    }
    return ret;
  };
  void make_manip_aux(Manip &);
  template <typename H, typename... T>
  void make_manip_aux(Manip &m, const H &h, const T &...t) {
    m.push_back(wrap(h));
    make_manip_aux(m, t...);
  }
  const auto make_manip = [](const auto &... args) -> Manip {
    Manip m(sizeof...(args));
    make_manip_aux(m, args...);
    return m;
  };
  const auto make_manip_seq = [](const auto &c) -> Manip {
    return foreach(wrap, c);
  };
  const auto endl = make_manip("\n");
  const auto colon = make_manip(": ");
  const auto comma = make_manip(", ");
  const auto parens = [](const Manip &m) -> Manip {
    return make_manip("(", m, ")");
  };
  const auto braces = [](const Manip &m) -> Manip {
    return make_manip("{", m, "}");
  };
  const auto squares = [](const Manip &m) -> Manip {
    return make_manip("[", m, "]");
  };
  const auto angles = [](const Manip &m) -> Manip {
    return make_manip("<", m, ">");
  };
  Manip join(const Manip &sep, const Manip &m);
  const auto set_like = [](const Manip &m) -> Manip {
    return braces(join(comma, m));
  };
  const auto key_value = [](const auto &x) -> Manip {
    return make_manip(std::get<0>(x), colon, std::get<1>(x));
  };

  template <bool B>
  using enabler = typename std::enable_if<B, std::nullptr_t>::type;
  template <typename T, typename U = llvm::raw_ostream &>
  using call_print = decltype(std::declval<T>().print(std::declval<U>()));
  template <typename T, typename = void>
  struct has_print : std::false_type {};
  template <typename T>
  struct has_print<T, std::void_t<call_print<T>>> : std::true_type {};

  template <class T, enabler<has_print<T>::value> = nullptr>
  llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const T &x) {
    x.print(O);
    return O;
  }
}

#endif//INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
