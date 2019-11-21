#ifndef INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
#define INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953

#include <unordered_set>

namespace llvm {
class Value;
}

namespace stacksafe {

class Params : private std::unordered_set<const llvm::Value *> {
  using Super = std::unordered_set<const llvm::Value *>;
  class ParamsIterator;

 public:
  ParamsIterator begin() const;
  ParamsIterator end() const;
  void emplace(const llvm::Value &v);
};

class Params::ParamsIterator : private Params::Super::const_iterator {
  friend class Params;
  using Base = Super::const_iterator;
  explicit ParamsIterator(Base it);
  Base &base();
  const Base &base() const;

 public:
  using Base::iterator_category, Base::value_type, Base::difference_type,
      Base::pointer, Base::reference;
  const llvm::Value &operator*() const;
  ParamsIterator &operator++();
  bool operator==(ParamsIterator it) const;
  bool operator!=(ParamsIterator it) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
