#include "env.hpp"
#include <algorithm>
#include "domain.hpp"

namespace stacksafe {

Domain Env::lookup(const Value& key) const {
  Domain dom;
  const auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.merge(it->second.get().lookup(key));
  }
  return dom;
}
bool Env::contains(const Value& key, const Value& val) const {
  const auto [lb, ub] = Super::equal_range(key);
  return range_contains(lb, ub, val);
}
bool Env::includes(const MapRef& ref) const {
  const auto pred = [& self = *this](const auto& pair) {
    return self.contains(pair.first, pair.second);
  };
  return std::all_of(ref.get().begin(), ref.get().end(), pred);
}
bool Env::range_contains(const_iterator lb, const_iterator ub,
                         const Value& val) {
  const auto pred = [&val](const auto& pair) {
    return pair.second.get().element(pair.first, val);
  };
  return std::any_of(lb, ub, pred);
}

}  // namespace stacksafe
