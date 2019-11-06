#include "domain.hpp"
#include <algorithm>
#include <iterator>
#include "json.hpp"

namespace stacksafe {

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
const Domain &Domain::get_empty() {
  static const Domain dom{};
  return dom;
}
const Domain &Domain::get_global() {
  static const auto dom = get_singleton(Symbol::get_global());
  return dom;
}
const Domain Domain::get_singleton(const Symbol &sym) {
  auto dom = get_empty();
  dom.Super::insert(sym);
  return dom;
}
void to_json(Json &j, const Domain &x) {
  static const std::string global{"@"};
  Json::array_t arr;
  for (const auto &e : x) {
    if (e.is_local()) {
      arr.push_back(get_operand(e.value()));
    } else {
      arr.push_back(global);
    }
  }
  j = arr;
}

}  // namespace stacksafe
