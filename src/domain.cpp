#include "domain.hpp"
#include <algorithm>
#include <iterator>
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

Domain::Domain(const Register &reg) : Super{reg} {}
void Domain::merge(const Domain &that) {
  Super::insert(that.begin(), that.end());
}
bool Domain::includes(const Domain &that) const {
  return std::includes(begin(), end(), that.begin(), that.end());
}
Domain Domain::minus(const Domain &that) const {
  Domain ret;
  auto inserter = std::inserter(static_cast<Super &>(ret), ret.end());
  std::set_difference(begin(), end(), that.begin(), that.end(), inserter);
  return ret;
}
bool Domain::has_local() const {
  for (const auto &sym : *this) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}
const Domain &Domain::get_empty() {
  static const Domain dom{};
  return dom;
}
const Domain &Domain::get_global() {
  static const Domain dom{Register::get_global()};
  return dom;
}
void to_json(Json &j, const Domain &x) {
  static const std::string global{"@"};
  Json::array_t arr;
  for (const auto &e : x) {
    if (auto v = e.value()) {
      arr.push_back(get_operand(*v));
    } else {
      arr.push_back(global);
    }
  }
  j = arr;
}

}  // namespace stacksafe
