#include "params.hpp"

namespace stacksafe {

Params::ParamsIterator::ParamsIterator(Base it) : Base{it} {}
auto Params::ParamsIterator::base() -> Base & {
  return *this;
}
auto Params::ParamsIterator::base() const -> const Base & {
  return *this;
}
const llvm::Value &Params::ParamsIterator::operator*() const {
  return **base();
}
auto Params::ParamsIterator::operator++() -> ParamsIterator & {
  ++base();
  return *this;
}
bool Params::ParamsIterator::operator==(ParamsIterator it) const {
  return base() == it.base();
}
bool Params::ParamsIterator::operator!=(ParamsIterator it) const {
  return base() != it.base();
}

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
