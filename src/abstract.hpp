#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <map>
#include "register.hpp"
#include "symbol.hpp"

namespace llvm {
class Function;
}

class Register;

class Abstraction {
  std::map<Register, Symbol> stack_;
  std::map<Symbol, Symbol> heap_;

 public:
  void interpret(llvm::Function& f);
  void alloc(const Register& r);
  void show() const;
};

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
