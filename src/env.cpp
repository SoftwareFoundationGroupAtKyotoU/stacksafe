#include "env.hpp"
#include <llvm/IR/Function.h>
#include "flat.hpp"
#include "pool.hpp"
#include "utility.hpp"

namespace stacksafe {

DoubleMap::DoubleMap(const llvm::Function &f) {
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
DoubleMap::DoubleMap(const Map &heap, const Map &stack)
    : heap_{heap}, stack_{stack} {}
const Map &DoubleMap::heap() const {
  return heap_;
}
Map &DoubleMap::heap() {
  return heap_;
}
const Map &DoubleMap::stack() const {
  return stack_;
}
Map &DoubleMap::stack() {
  return stack_;
}
bool DoubleMap::includes(const DoubleMap &that) const {
  return heap_.includes(that.heap_) && stack_.includes(that.stack_);
}
void DoubleMap::merge(const DoubleMap &that) {
  heap_.merge(that.heap_);
  stack_.merge(that.stack_);
}
FlatEnv DoubleMap::to_flat_env() const {
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
void FlatEnv::merge(const FlatEnv &flat) {
  heap_.merge(flat.heap_);
  stack_.merge(flat.stack_);
}

Env::Env(FlatMapRef heap, FlatMapRef stack) : heap_{heap}, stack_{stack} {}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
bool Env::includes(const Env &env) {
  auto compare = [](const auto &lhs, const auto &rhs) {
    FlatMap lmap, rmap;
    for (const auto &ref : lhs) {
      if (rhs.count(ref) == 0) {
        lmap.merge(ref.get());
      }
    }
    for (const auto &ref : rhs) {
      if (lhs.count(ref) == 0) {
        rmap.merge(ref.get());
      }
    }
    return lmap.includes(rmap);
  };
  return compare(heap_, env.heap_) && compare(stack_, env.stack_);
}
DoubleMap Env::concat() const {
  Map heap, stack;
  for (const auto &r : heap_) {
    heap.merge(FlatMap::to_map(r.get()));
  }
  for (const auto &r : stack_) {
    stack.merge(FlatMap::to_map(r.get()));
  }
  return DoubleMap{heap, stack};
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
DoubleMap Env::to_map() const {
  Map heap, stack;
  for (const auto &r : heap_) {
    heap.merge(FlatMap::to_map(r.get()));
  }
  for (const auto &r : stack_) {
    stack.merge(FlatMap::to_map(r.get()));
  }
  return DoubleMap{heap, stack};
}

}  // namespace stacksafe
