#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <map>
#include "memory.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {
class Register;

class Abstraction {
  Stack stack_;
  Heap heap_;

 public:
  void interpret(llvm::Function& f);
  void argument(const Register& arg);
  void alloc(const Register& r);
  void store(const Register& src, const Register& dst);
  void show() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
