#include "fabric.hpp"
#include <utility>
#include "utility.hpp"

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
Fabric& Fabric::next() {
  ++pos_;
  return *this;
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
Fabric& Fabric::patch(const Fabric& fab) {
  const auto space = ' ';
  auto& left = fabric_;
  auto& right = fab.fabric_;
  std::size_t width = 0;
  for (auto& line : left) {
    width = width < line.size() ? line.size() : width;
  }
  auto i = left.size();
  for (; i < right.size(); ++i) {
    left.emplace_back("");
  }
  i = 0;
  for (auto& line : left) {
    if (i < right.size()) {
      line.append(width - line.size(), space);
      line.append(right.at(i));
    }
    ++i;
  }
  return *this;
}
void Fabric::print(llvm::raw_ostream& os) const {
  bool first = true;
  for (auto& line : fabric_) {
    if (!std::exchange(first, false)) {
      endline(os);
    }
    print_string(os, line);
  }
}
llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Fabric& fab) {
  fab.print(os);
  return os;
}

}  // namespace stacksafe
