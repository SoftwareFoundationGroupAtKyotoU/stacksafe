#include "cache.hpp"
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
std::optional<int> to_int(const llvm::Value& v) {
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

Cache::Cache() : cache_{std::make_unique<Super>()} {
  assert(cache_ && "failed cache init");
  cache_->try_emplace(nullptr, -1);
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
  if (auto it = cache_->find(val); it != cache_->end()) {
    return it->second;
  } else {
    auto v = val.value();
    assert(v && "null is always in cache");
    auto num = to_int(*v);
    assert(num && "not a register");
    cache_->try_emplace(val, *num);
    return *num;
  }
}

}  // namespace stacksafe
