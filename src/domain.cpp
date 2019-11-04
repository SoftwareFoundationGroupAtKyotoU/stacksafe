#include "domain.hpp"
#include <algorithm>
#include <iterator>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

Domain::Domain(std::initializer_list<Register> list) : Super{list} {}
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
  Json::array_t arr;
  for (const auto &e : x) {
    arr.push_back(to_str(e));
  }
  j = arr;
}
Fabric dump(const Domain &domain) {
  Fabric ret;
  bool first = true;
  for (const auto &reg : domain) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(to_str(reg));
  }
  return ret.quote("[", "]");
}
std::string to_str(const Domain &dom) {
  std::string ret;
  bool first = true;
  for (const auto &reg : dom) {
    if (!std::exchange(first, false)) {
      ret.append(", ");
    }
    ret.append(to_str(reg));
  }
  ret.insert(0, "[");
  ret.append("]");
  return ret;
}

}  // namespace stacksafe
