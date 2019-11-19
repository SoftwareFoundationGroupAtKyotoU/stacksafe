#include "env.hpp"
#include <llvm/IR/Function.h>
#include "flat.hpp"
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
FlatEnv FlatEnvOld::to_flat_env() const {
  FlatMap heap, stack;
  heap.insert(heap_);
  stack.insert(stack_);
  return FlatEnv{heap, stack};
}

FlatEnv::FlatEnv(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  heap_.insert(g.value(), g);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    heap_.insert(arg.value(), arg);
    stack_.insert(a, arg);
  }
}
FlatEnv::FlatEnv(const FlatMap &heap, const FlatMap &stack)
    : heap_{heap}, stack_{stack} {}
const FlatMap &FlatEnv::heap() const {
  return heap_;
}
FlatMap &FlatEnv::heap() {
  return heap_;
}
const FlatMap &FlatEnv::stack() const {
  return stack_;
}
FlatMap &FlatEnv::stack() {
  return stack_;
}
bool FlatEnv::includes(const FlatEnv &flat) const {
  return heap_.includes(flat.heap_) && stack_.includes(flat.stack_);
}

Env::Env(FlatMapRef heap, FlatMapRef stack) : heap_{heap}, stack_{stack} {}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
FlatEnvOld Env::concat() const {
  Map heap, stack;
  for (const auto &r : heap_) {
    heap.merge(FlatMap::to_map(r.get()));
  }
  for (const auto &r : stack_) {
    stack.merge(FlatMap::to_map(r.get()));
  }
  return FlatEnvOld{heap, stack};
}
FlatEnv Env::flatten() const {
  FlatMap heap, stack;
  for (const auto &r : heap_) {
    heap.merge(r.get());
  }
  for (const auto &r : stack_) {
    stack.merge(r.get());
  }
  return FlatEnv{heap, stack};
}

}  // namespace stacksafe
