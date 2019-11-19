#include "env.hpp"
#include <llvm/IR/Function.h>
#include "pool.hpp"
#include "utility.hpp"

namespace stacksafe {

FlatEnvOld::FlatEnvOld(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  Domain dom;
  dom.insert(g);
  heap_.insert(g.value(), dom);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    Domain dom;
    dom.insert(arg);
    heap_.insert(arg.value(), dom);
    stack_.insert(a, dom);
  }
}
FlatEnvOld::FlatEnvOld(const Map &heap, const Map &stack)
    : heap_{heap}, stack_{stack} {}
const Map &FlatEnvOld::heap() const {
  return heap_;
}
Map &FlatEnvOld::heap() {
  return heap_;
}
const Map &FlatEnvOld::stack() const {
  return stack_;
}
Map &FlatEnvOld::stack() {
  return stack_;
}
bool FlatEnvOld::includes(const FlatEnvOld &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void FlatEnvOld::merge(const FlatEnvOld &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}

EnvOld::EnvOld(MapRef heap, MapRef stack) : heap_{heap}, stack_{stack} {}
void EnvOld::merge(const EnvOld &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
FlatEnvOld EnvOld::concat() const {
  Map heap, stack;
  for (const auto &m : heap_) {
    heap.merge(m.get());
  }
  for (const auto &m : stack_) {
    stack.merge(m.get());
  }
  return FlatEnvOld{heap, stack};
}

FlatEnv::FlatEnv(const FlatMap &heap, const FlatMap &stack)
    : heap_{heap}, stack_{stack} {}
FlatMap &FlatEnv::heap() {
  return heap_;
}
FlatMap &FlatEnv::stack() {
  return stack_;
}
bool FlatEnv::includes(const FlatEnv &flat) const {
  return heap_.includes(flat.heap_) && stack_.includes(flat.stack_);
}

Env::Env(FlatMapRef heap, FlatMapRef stack) : heap_{heap}, stack_{stack} {}

}  // namespace stacksafe
