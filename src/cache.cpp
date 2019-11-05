#include "cache.hpp"
#include <optional>
#include <string_view>
#include "register.hpp"
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
int Cache::lookup(const llvm::Value* reg) const {
  if (auto it = cache_->find(reg); it != cache_->end()) {
    return it->second;
  } else {
    assert(reg && "null is always in cache");
    auto num = to_int(*reg);
    assert(num && "not a register");
    cache_->try_emplace(reg, *num);
    return *num;
  }
}
std::string Cache::to_str(const llvm::Value* reg) const {
  static const std::string global{"@"};
  if (auto num = lookup(reg); num < 0) {
    return global;
  } else {
    return std::to_string(num);
  }
}
std::string Cache::to_str(const Register& reg) const {
  static const std::string prefix{"&"};
  return prefix + to_str(reg.value());
}
std::string Cache::to_str(const llvm::Value& reg) const {
  static const std::string prefix{"%"};
  return prefix + to_str(&reg);
}

}  // namespace stacksafe
