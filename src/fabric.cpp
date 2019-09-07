#include "fabric.hpp"

namespace stacksafe {

Fabric::Fabric() : pos_{0} {}
std::string& Fabric::current() {
  if (pos_ < fabric_.size()) {
    return fabric_.at(pos_);
  } else {
    fabric_.emplace_back("");
    return current();
  }
}

}  // namespace stacksafe
