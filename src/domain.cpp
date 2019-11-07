#include "domain.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

void Domain::merge(const Domain &that) {
  Super::insert(that.begin(), that.end());
}
bool Domain::includes(const Domain &that) const {
  for (const auto &sym : that) {
    if (Super::count(sym) == 0) {
      return false;
    }
  }
  return true;
}
Domain Domain::minus(const Domain &that) const {
  Domain ret;
  for (const auto &sym : *this) {
    if (that.Super::count(sym) == 0) {
      ret.Super::insert(sym);
    }
  }
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
