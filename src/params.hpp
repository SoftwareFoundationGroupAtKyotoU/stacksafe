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

class Params::ParamsIterator : private Super::const_iterator {
  friend class Params;
  using Base = Super::const_iterator;
  explicit ParamsIterator(Base it) : Base{it} {}
  Base &base() { return *this; }
  const Base &base() const { return *this; }

 public:
  using Base::iterator_category, Base::value_type, Base::difference_type,
      Base::pointer, Base::reference;
  const llvm::Value &operator*() const { return **base(); }
  ParamsIterator &operator++() {
    ++base();
    return *this;
  }
  bool operator==(ParamsIterator it) const { return base() == it.base(); }
  bool operator!=(ParamsIterator it) const { return base() != it.base(); }
};

auto Params::begin() const -> ParamsIterator {
  return ParamsIterator{Super::begin()};
}
auto Params::end() const -> ParamsIterator {
  return ParamsIterator{Super::end()};
}
void Params::emplace(const llvm::Value &v) {
  Super::emplace(&v);
}
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
