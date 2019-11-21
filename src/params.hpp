#ifndef INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
#define INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953

#include <vector>

namespace llvm {
class Value;
}

namespace stacksafe {

class Params : private std::vector<const llvm::Value *> {
  using Super = std::vector<const llvm::Value *>;
  class ParamsIterator;

 public:
  ParamsIterator begin() const;
  ParamsIterator end() const;
  void push_back(const llvm::Value &v);
};

class Params::ParamsIterator : private Params::Super::const_iterator {
  friend class Params;
  using Super = Super::const_iterator;
  explicit ParamsIterator(Super it);
  Super &super();
  const Super &super() const;

 public:
  using Super::iterator_category, Super::value_type, Super::difference_type,
      Super::pointer, Super::reference;
  const llvm::Value &operator*() const;
  ParamsIterator &operator++();
  bool operator==(ParamsIterator it) const;
  bool operator!=(ParamsIterator it) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
