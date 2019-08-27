#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <map>
#include "memory.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {

class Abstraction {
  Stack stack_;
  Heap heap_;

 public:
  void alloc(const Register& dst);
  void store(const Register& src, const Register& dst);
  void load(const Register& dst, const Register& src);
  void show() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
