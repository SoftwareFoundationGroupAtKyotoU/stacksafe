#include "env.hpp"
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

}  // namespace stacksafe
