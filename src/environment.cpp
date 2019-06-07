#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
Env_::Env_(LocationFactory &factory) : factory_(factory) {
  auto g = LocationFactory::getGlobal();
  auto o = LocationFactory::getOutlive();
  if (heap_.insert(g, g) || heap_.insert(o, LocationSet{{g, o}})) {
    llvm::errs() << "Error: unreachable" << endl;
  }
}
bool Env_::subsetof(const Env_ &rhs) const {
  return (heap_.subsetof(rhs.heap_) && stack_.subsetof(rhs.stack_));
}
void Env_::unify(const Env_ &rhs) {
  heap_.insert(rhs.heap_);
  stack_.insert(rhs.stack_);
}
void Env_::print(llvm::raw_ostream &O) const {
  O << "heap: " << heap_ << endl;
  O << "stack: " << stack_ << endl;
}

bool Env_::init_argument(const Register &dst) {
  if (auto &val = dst.get(); llvm::isa<llvm::Argument>(val)) {
    if (llvm::isa<llvm::PointerType>(val.getType())) {
      auto o = LocationFactory::getOutlive();
      auto g = LocationFactory::getGlobal();
      return !stack_.insert(dst, LocationSet{{g, o}});
    } else {
      return !stack_.insert(dst);
    }
  }
  return false;
}
bool Env_::alloca(const Register &dst) {
  auto l = factory_.getLocal();
  return !(heap_.insert(l) || stack_.insert(dst, l));
}
bool Env_::store(const Value &src, const Register &dst) {
  if (auto val = to_symbols(src)) {
    if (auto ptr = stack_.get(dst)) {
      for (auto &each : *ptr) {
        if (heap_.insert(each, *val)) {
          continue;
        }
        return false;
      }
      return true;
    }
  }
  return false;
}
bool Env_::load(const Register &dst, const Register &src) {
  if (auto ptr = stack_.get(src)) {
    for (auto &each : *ptr) {
      if (auto val = heap_.get(each)) {
        stack_.insert(dst, *val);
        continue;
      }
      return false;
    }
    return true;
  }
  return false;
}
bool Env_::getelementptr(const Register &dst, const Register &src) {
  if (auto val = stack_.get(src)) {
    stack_.insert(dst, *val);
    return true;
  }
  return false;
}
bool Env_::binary(const Register &dst) {
  stack_.insert(dst);
  return true;
}
bool Env_::cast(const Register &dst, const Value &src) {
  if (llvm::isa<llvm::Constant>(src.get())) {
    stack_.insert(dst);
    return true;
  } else if (auto reg = make_register(src)) {
    if (auto val = stack_.get(*reg)) {
      stack_.insert(dst, *val);
      return true;
    }
  }
  return false;
}
bool Env_::phi(const Register &dst, const Value &src) {
  if (llvm::isa<llvm::Constant>(src.get())) {
    stack_.insert(dst);
    return true;
  } else if (auto reg = make_register(src)) {
    if (auto val = stack_.get(*reg)) {
      stack_.insert(dst, *val);
      return true;
    }
  }
  return false;
}
bool Env_::cmp(const Register &dst) {
  stack_.insert(dst);
  return true;
}
bool Env_::call(const UserRange &args, std::optional<Register> dst) {
  LocationSet reachs{{LocationFactory::getGlobal()}};
  for (auto &arg : args) {
    if (auto reg = make_register(*arg.get())) {
      if (auto next = stack_.get(*reg)) {
        if (reach(*next, reachs)) {
          continue;
        }
      }
      return false;
    }
  }
  for (auto &r : reachs) {
    if (heap_.insert(r, reachs)) {
      continue;
    }
    return false;
  }
  if (dst) {
    return !stack_.insert(*dst, reachs);
  }
  return true;
}

std::optional<LocationSet> Env_::to_symbols(const Value &v) const {
  const auto ptr = &v.get();
  if (auto reg = make_register(v)) {
    if (auto val = stack_.get(*reg)) {
      return *val;
    }
  } else if (llvm::isa<llvm::ConstantPointerNull>(ptr)) {
    if (auto val = heap_.get(LocationFactory::getGlobal())) {
      return *val;
    }
  } else if (llvm::isa<llvm::Constant>(ptr)) {
    return LocationSet{};
  }
  return std::nullopt;
}
bool Env_::reach(const LocationSet &locs, LocationSet &reachs) const {
  reachs.insert(locs);
  for (auto &loc : locs) {
    if (auto next = heap_.get(loc)) {
      if (next->subsetof(reachs) || reach(*next, reachs)) {
        continue;
      }
    }
    return false;
  }
  return true;
}
} // namespace stacksafe
