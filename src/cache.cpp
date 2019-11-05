#include "cache.hpp"
#include "utility.hpp"

namespace stacksafe {

Cache::Cache() : cache_{std::make_unique<Super>()} {}
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
