#include "fabric.hpp"
#include <llvm/Support/raw_ostream.h>
#include <utility>

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
  bool first = true;
  for (auto& line : fab.fabric_) {
    if (!std::exchange(first, false)) {
      next();
    }
    append(line);
  }
  return *this;
}
Fabric& Fabric::prepend(const std::string& str) {
  current().insert(0, str);
  return *this;
}
Fabric& Fabric::prepend(const Fabric& fab) {
  bool first = true;
  for (auto& line : fab.fabric_) {
    if (!std::exchange(first, false)) {
      next();
    }
    prepend(line);
  }
  return *this;
}
Fabric& Fabric::quote(const std::string& open, const std::string& close) {
  return prepend(open).append(close);
}
Fabric& Fabric::quote() {
  const auto q = R"(")";
  return quote(q, q);
}
Fabric& Fabric::indent(std::size_t width) {
  const auto space = ' ';
  for (auto& line : fabric_) {
    line.insert(0, width, space);
  }
  return *this;
}
void Fabric::print(llvm::raw_ostream& os) const {
  bool first = true;
  for (auto& line : fabric_) {
    if (!std::exchange(first, false)) {
      os << "\n";
    }
    os << line;
  }
}
llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Fabric& fab) {
  fab.print(os);
  return os;
}

}  // namespace stacksafe
