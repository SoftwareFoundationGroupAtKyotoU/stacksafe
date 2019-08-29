#include "memory.hpp"

namespace stacksafe {
namespace {
template <typename Env>
void env_to_json(Json& j, const Env& x) {
  auto to_str = [](const auto& k) {
    Json tmp;
    to_json(tmp, k);
    return tmp.get<std::string>();
  };
  Json tmp;
  for (auto& [k, v] : x) {
    tmp[to_str(k)] = v;
  }
  j = tmp;
}
}  // namespace
}  // namespace stacksafe
