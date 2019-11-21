#include "cache.hpp"
#include <llvm/IR/Function.h>
#include <vector>
#include "domain.hpp"
#include "utility.hpp"

namespace stacksafe {
std::string to_string(int num) {
  static const std::string global{"@"};
  if (num < 0) {
    return global;
  } else {
    return std::to_string(num);
  }
}

Cache::Cache(const llvm::Function& f) {
  int num = -1;
  Super::try_emplace(Value{}, num++);
  for (const auto& a : f.args()) {
    Super::try_emplace(a, num++);
  }
  for (const auto& b : f) {
    ++num;
    for (const auto& i : b) {
      if (is_register(i)) {
        Super::try_emplace(i, num++);
      }
    }
  }
}
std::string Cache::to_str(const Symbol& sym) const {
  static const std::string prefix{"&"};
  return prefix + to_string(lookup(sym.value()));
}
std::string Cache::to_str(const llvm::Instruction& reg) const {
  static const std::string prefix{"%"};
  return prefix + to_string(lookup(reg));
}
std::string Cache::to_str(const Domain& dom) const {
  const auto cmp = [& self = *this](const auto& lhs, const auto& rhs) {
    return self.lookup(lhs.value()) < self.lookup(rhs.value());
  };
  std::vector<Symbol> symbols{dom.begin(), dom.end()};
  std::sort(symbols.begin(), symbols.end(), cmp);
  std::string ret{"["};
  bool first = true;
  for (const auto& sym : symbols) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(to_str(sym));
  }
  ret.append("]");
  return ret;
}
int Cache::lookup(const Value& val) const {
  auto it = Super::find(val);
  assert(it != Super::end() && "unregistered value");
  return it->second;
}

}  // namespace stacksafe
