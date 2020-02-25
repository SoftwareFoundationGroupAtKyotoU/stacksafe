#include "effect.hpp"
#include <string_view>
#include <vector>

namespace stacksafe {
namespace {
std::vector<std::string_view> split(std::string_view v, const char* delim) {
  std::vector<std::string_view> vec;
  const auto next = [&v, delim]() { return v.find_first_of(delim); };
  for (auto pos = next(); pos != v.npos; pos = next()) {
    vec.emplace_back(v.substr(0, pos));
    v.remove_prefix(pos + 1);
  }
  vec.emplace_back(v);
  return vec;
}
}  // namespace
}  // namespace stacksafe
