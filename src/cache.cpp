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
    auto num = to_int(view.substr(1));
    assert(num && 0 <= *num);
    return *num;
  }
  return std::nullopt;
}
}  // namespace

Cache::Cache() : cache_{std::make_unique<Super>()} {
  assert(cache_ && "failed cache init");
  cache_->try_emplace(nullptr, -1);
}
std::optional<int> Cache::lookup(const llvm::Value& reg) const {
  if (cache_) {
    if (auto it = cache_->find(&reg); it != cache_->end()) {
      return it->second;
    } else if (auto num = to_int(reg)) {
      cache_->try_emplace(&reg, *num);
      return *num;
    }
  }
  return std::nullopt;
}
std::string Cache::to_str(const Register& reg) const {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (!reg.is_local()) {
    return prefix + global;
  } else if (auto num = lookup(*reg.value())) {
    return prefix + std::to_string(*num);
  } else {
    return "[unknown]";
  }
}
std::string Cache::to_str(const llvm::Value& reg) const {
  static const std::string prefix{"%"};
  if (auto num = lookup(reg)) {
    return prefix + std::to_string(*num);
  } else {
    return "[unknown]";
  }
}

}  // namespace stacksafe
