#include "cache.hpp"
#include "utility.hpp"

namespace stacksafe {

int Cache::lookup(const Register& reg) {
  if (!reg.is_local()) {
    return -1;
  } else if (auto it = Super::find(reg); it != Super::end()) {
    return it->second;
  } else if (auto num = register_number(*reg.value())) {
    Super::try_emplace(reg, *num);
    return *num;
  } else {
    return -2;
  }
}
std::string Cache::to_str(const Register& reg) {
  return std::to_string(lookup(reg));
}

}  // namespace stacksafe
