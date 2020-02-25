#include "effect.hpp"
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
}  // namespace
}  // namespace stacksafe
