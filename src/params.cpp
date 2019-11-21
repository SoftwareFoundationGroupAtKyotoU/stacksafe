#include "params.hpp"

namespace stacksafe {

Params::ParamsIterator::ParamsIterator(Super it) : Super{it} {}
auto Params::ParamsIterator::super() -> Super & {
  return *this;
}
auto Params::ParamsIterator::super() const -> const Super & {
  return *this;
}
const llvm::Value &Params::ParamsIterator::operator*() const {
  return **super();
}
auto Params::ParamsIterator::operator++() -> ParamsIterator & {
  ++super();
  return *this;
}
bool Params::ParamsIterator::operator==(ParamsIterator it) const {
  return super() == it.super();
}
bool Params::ParamsIterator::operator!=(ParamsIterator it) const {
  return super() != it.super();
}

auto Params::begin() const -> ParamsIterator {
  return ParamsIterator{Super::begin()};
}
auto Params::end() const -> ParamsIterator {
  return ParamsIterator{Super::end()};
}
void Params::push_back(const llvm::Value &v) {
  Super::push_back(&v);
}

}  // namespace stacksafe
