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
Fabric& Fabric::append(const Fabric& fab) {
  auto prev = pos_;
  for (auto& line : fab.fabric_) {
    append(line).next();
  }
  pos_ = prev;
  return *this;
}
Fabric& Fabric::prepend(const std::string& str) {
  current().insert(0, str);
  return *this;
}
Fabric& Fabric::quote(const std::string& open, const std::string& close) {
  return prepend(open).append(close);
}

}  // namespace stacksafe
