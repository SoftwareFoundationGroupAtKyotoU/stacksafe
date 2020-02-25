#include "effect.hpp"
#include <climits>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace stacksafe {
namespace {
class EffectLine {
  using Views = std::vector<std::string_view>;
  std::string name_;
  std::size_t arity_;

 private:
  static Views split(std::string_view v, const char* delim);
  static std::optional<std::size_t> to_size_t(std::string_view v);
};
auto EffectLine::split(std::string_view v, const char* delim) -> Views {
  Views views;
  const auto next = [&v, delim]() { return v.find_first_of(delim); };
  for (auto pos = next(); pos != v.npos; pos = next()) {
    views.emplace_back(v.substr(0, pos));
    v.remove_prefix(pos + 1);
  }
  views.emplace_back(v);
  return views;
  ;
}
std::optional<std::size_t> EffectLine::to_size_t(std::string_view v) {
  std::string buf{v};
  const auto b = buf.c_str();
  const auto e = b + buf.size();
  if (b != e) {
    char* p = nullptr;
    const auto val = std::strtol(b, &p, 10);
    if (p == e && 0 <= val && val != LONG_MAX) {
      return val;
    }
  }
  return std::nullopt;
}
}  // namespace
}  // namespace stacksafe
