#include "cache.hpp"
#include "utility.hpp"

namespace stacksafe {

Cache::Cache() : cache_{std::make_unique<Super>()} {}
std::optional<int> Cache::lookup(const llvm::Value& reg) const {
  if (cache_) {
    if (auto it = cache_->find(Register::get_local(reg)); it != cache_->end()) {
      return it->second;
    } else if (auto num = register_number(reg)) {
      cache_->try_emplace(Register::get_local(reg), *num);
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

}  // namespace stacksafe
