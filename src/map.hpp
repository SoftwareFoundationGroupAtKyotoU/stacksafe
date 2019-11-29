#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <set>
#include <unordered_map>
#include "domain.hpp"
#include "hash.hpp"
#include "value.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Register;
class Symbol;

class Map : private std::unordered_map<Value, Domain> {
  using Super = std::unordered_map<Value, Domain>;

 public:
  using Super::begin, Super::end;
  void init(const llvm::Function &f);
  bool insert(const Symbol &key, const Domain &dom);
  bool insert(const Register &key, const Domain &dom);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Register &key) const;
  void merge(const Map &map);
  static std::set<Value> keys(const Map &map);

 private:
  Domain find(const Value &key) const;
  Domain &get(const Value &key);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
