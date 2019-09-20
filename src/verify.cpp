#include "verify.hpp"
#include "env.hpp"
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Verifier::Verifier(const Env &e) : env_{e} {}
auto Verifier::run(const llvm::BasicBlock *b, const Env &env) -> RetTy {
  auto dom = env.from_heap(Symbol::global());
  if (dom.has_local()) {
    return unsafe;
  }
  return Verifier{env}.visit(*b);
}
auto Verifier::visit(const llvm::BasicBlock &b) -> RetTy {
  for (auto &i : const_cast<llvm::BasicBlock &>(b)) {
    if (!Super::visit(i)) {
      return unsafe;
    }
  }
  return safe;
}
auto Verifier::visitCallInst(llvm::CallInst &i) -> RetTy {
  for (auto &use : i.args()) {
    if (auto arg = use.get()) {
      auto dom = env_.from_stack(Value::make(*arg));
      if (dom.has_local() && dom.includes(Symbol::global())) {
        return unsafe;
      }
    } else {
      stacksafe_unreachable("unknown parameter", i);
    }
  }
  return safe;
}
auto Verifier::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto r = i.getReturnValue()) {
    auto v = Value::make(*r);
    if (v.is_register()) {
      if (auto dom = env_.stack().get(v)) {
        if (dom->has_local()) {
          return unsafe;
        }
      }
    }
  }
  return safe;
}
auto Verifier::visitInstruction(llvm::Instruction &) -> RetTy { return safe; }

}  // namespace stacksafe
