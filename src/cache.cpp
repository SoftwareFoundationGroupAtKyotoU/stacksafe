#include "cache.hpp"
#include <llvm/IR/Function.h>
#include <cassert>
#include <optional>
#include <set>
#include <string_view>
#include "domain.hpp"
#include "register.hpp"
#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {
namespace {
std::optional<int> to_int(std::string_view view) {
  if (!view.empty()) {
    std::string str{view};
    std::size_t pos = std::string_view::npos;
    auto val = std::stoi(str, &pos, 10);
    if (pos == view.size()) {
      return val;
    }
  }
  return std::nullopt;
}
std::optional<int> to_int(const Value& v) {
  static const auto prefix = '%';
  auto operand = get_operand(v);
  std::string_view view{operand};
  if (!view.empty() && view.at(0) == prefix) {
    if (auto num = to_int(view.substr(1))) {
      assert(0 <= *num && "invalid register number");
      return *num;
    };
  }
  return std::nullopt;
}
}  // namespace

Cache::Cache(const llvm::Function& f) : cache_{std::make_unique<Super>()} {
  assert(cache_ && "failed cache init");
  int num = -1;
  cache_->try_emplace(Value{}, num++);
  for (const auto& a : f.args()) {
    cache_->try_emplace(a, num++);
  }
  for (const auto& b : f) {
    ++num;
    for (const auto& i : b) {
      if (is_register(i)) {
        cache_->try_emplace(i, num++);
      }
    }
  }
}
std::string Cache::to_str(const Symbol& reg) const {
  static const std::string prefix{"&"};
  return prefix + to_str(lookup(reg.value()));
}
std::string Cache::to_str(const Register& reg) const {
  static const std::string prefix{"%"};
  return prefix + to_str(lookup(reg.value()));
}
std::string Cache::to_str(const Domain& dom) const {
  static const std::string prefix{"&"};
  static const std::string comma{", "};
  static const std::string begin{"["};
  static const std::string end{"]"};
  std::set<int> nums;
  for (const auto& reg : dom) {
    nums.insert(lookup(reg.value()));
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
  auto it = cache_->find(val);
  assert(it != cache_->end() && "unregistered value");
  return it->second;
}

}  // namespace stacksafe
