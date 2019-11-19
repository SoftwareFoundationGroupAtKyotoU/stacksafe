#include "flat.hpp"
#include "utility.hpp"

namespace stacksafe {

MapsTo::MapsTo(const Value& val, const Symbol& sym) : Super{val, sym} {}
const Value& MapsTo::value() const {
  return std::get<0>(*this);
}
const Symbol& MapsTo::symbol() const {
  return std::get<1>(*this);
}
std::size_t MapsTo::hash(const MapsTo& to) {
  return hash_combine(Value::hash(to.value()), Symbol::hash(to.symbol()));
}
bool operator==(const MapsTo& lhs, const MapsTo& rhs) {
  return lhs.value() == rhs.value() && lhs.symbol() == rhs.symbol();
}

}  // namespace stacksafe
