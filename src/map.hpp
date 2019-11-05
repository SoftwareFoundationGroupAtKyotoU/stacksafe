#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include "domain.hpp"
#include "json_fwd.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {
class Register;

class Map : private std::map<const llvm::Value *, Domain> {
  using Super = std::map<const llvm::Value *, Domain>;

 public:
  using Super::begin, Super::end;
  void insert(const Register &key, const Domain &val);
  const Domain &lookup(const Register &key) const;
  void merge(const Map &that);
  bool includes(const Map &that) const;
};
void to_json(Json &j, const Map &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
