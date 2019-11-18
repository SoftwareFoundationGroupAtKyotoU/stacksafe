#include "cache.hpp"
#include <llvm/IR/Function.h>
#include <set>
#include "domain.hpp"
#include "utility.hpp"

namespace stacksafe {

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
std::string Cache::to_str(const Value& val) const {
  return to_str(lookup(val));
}
std::string Cache::to_str(const Domain& dom) const {
  static const std::string prefix{"&"};
  static const std::string comma{", "};
  static const std::string begin{"["};
  static const std::string end{"]"};
  std::set<int> nums;
  for (const auto& sym : dom) {
    nums.insert(lookup(sym.value()));
  }
  std::string ret;
  bool first = true;
  for (const auto& num : nums) {
    if (!std::exchange(first, false)) {
      ret.append(comma);
    }
    ret.append(prefix + to_str(num));
  }
  ret.insert(0, begin);
  ret.append(end);
  return ret;
}
std::string Cache::to_str(int num) {
  static const std::string global{"@"};
  if (num < 0) {
    return global;
  } else {
    return std::to_string(num);
  }
}
int Cache::lookup(const Value& val) const {
  auto it = Super::find(val);
  assert(it != Super::end() && "unregistered value");
  return it->second;
}

}  // namespace stacksafe
