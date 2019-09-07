#include "fabric.hpp"

namespace stacksafe {

Fabric::Fabric() : pos_{0} {}
void Fabric::next() { ++pos_; }
std::string& Fabric::current() {
  if (pos_ < fabric_.size()) {
    return fabric_.at(pos_);
  } else {
    fabric_.emplace_back("");
    return current();
  }
}
Fabric& Fabric::append(const std::string& str) {
  current().append(str);
  return *this;
}

}  // namespace stacksafe
