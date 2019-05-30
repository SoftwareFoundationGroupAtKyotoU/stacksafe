#ifndef INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
#define INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5

#include <functional>
#include <llvm/IR/InstVisitor.h>
#include <llvm/Support/raw_ostream.h>
#include <type_traits>
#include <vector>

namespace llvm {
#define HANDLE_INST(NUM, OPCODE, CLASS) class CLASS;
#include <llvm/IR/Instruction.def>
#undef HANDLE_INST
} // namespace llvm

namespace stacksafe {
class ClassNameVisitor
    : public llvm::InstVisitor<ClassNameVisitor, const char *> {
  using RetTy = const char *;

public:
#define HANDLE_INST(NUM, OPCODE, CLASS)                                        \
  RetTy visit##OPCODE(llvm::CLASS &I) { return #CLASS ": " #OPCODE; }
#include <llvm/IR/Instruction.def>
#undef HANDLE_INST
};

using ManipObj = std::function<void(llvm::raw_ostream &)>;
class Manipulator : std::vector<ManipObj> {
  using Base = std::vector<ManipObj>;
  template <typename T> static ManipObj wrap(const T &t) {
    return [&t](llvm::raw_ostream &O) { O << t; };
  }
  static ManipObj wrap(const ManipObj &m) { return m; }

public:
  using Base::begin, Base::end;
  Manipulator();
  void print(llvm::raw_ostream &O) const;
  void manip_impl();
  template <typename H, typename... T>
  void manip_impl(const H &h, const T &... t) {
    add(h);
    manip_impl(t...);
  }
  template <typename T> void add(const T &t) { Base::push_back(wrap(t)); }
  template <typename T> Manipulator join(const T &t) const {
    Manipulator m;
    bool first = false;
    for (auto &e : *this) {
      if (std::exchange(first, true)) {
        m.add(t);
      }
      m.Base::push_back(e);
    }
    return m;
  }
};
template <typename... T> Manipulator manip(const T &... t) {
  Manipulator m;
  m.manip_impl(t...);
  return m;
}
template <typename F, typename C> Manipulator for_each(const F &f, const C &c) {
  Manipulator m;
  for (auto &e : c) {
    m.add(f(e));
  }
  return m;
}
template <typename C> Manipulator split(const C &c) {
  Manipulator m;
  for (auto &e : c) {
    m.add(e);
  }
  return m;
}

inline const auto endl = manip("\n");
inline const auto colon = manip(": ");
inline const auto comma = manip(", ");
inline const auto spaces = [](const auto &m) { return manip(" ", m, " "); };
inline const auto parens = [](const auto &m) { return manip("(", m, ")"); };
inline const auto braces = [](const auto &m) { return manip("{", m, "}"); };
inline const auto squares = [](const auto &m) { return manip("[", m, "]"); };
inline const auto angles = [](const auto &m) { return manip("<", m, ">"); };
inline const auto key_value = [](const auto &p) {
  return manip(std::get<0>(p), colon, std::get<1>(p));
};
inline const auto set_like = [](const auto &c) {
  return braces(split(c).join(comma));
};

template <bool B>
using enabler = typename std::enable_if<B, std::nullptr_t>::type;
template <typename T, typename U = llvm::raw_ostream &>
using call_print = decltype(std::declval<T>().print(std::declval<U>()));
template <typename T, typename = void> struct has_print : std::false_type {};
template <typename T>
struct has_print<T, std::void_t<call_print<T>>> : std::true_type {};

template <class T, enabler<has_print<T>::value> = nullptr>
llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const T &x) {
  x.print(O);
  return O;
}

llvm::raw_ostream &debugs(const char *type = "");
} // namespace stacksafe

#endif // INCLUDE_GUARD_30B970DD_A9A2_4C3C_B2A3_482271B1A2C5
