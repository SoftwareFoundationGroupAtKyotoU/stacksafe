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
int Cache::lookup(const Register& reg) {
  if (!reg.is_local()) {
    return -1;
  } else if (auto it = cache_->find(reg); it != cache_->end()) {
    return it->second;
  } else if (auto num = register_number(*reg.value())) {
    cache_->try_emplace(reg, *num);
    return *num;
  } else {
    return -2;
  }
}
std::string Cache::to_str(const Register& reg) {
  return std::to_string(lookup(reg));
}

}  // namespace stacksafe
