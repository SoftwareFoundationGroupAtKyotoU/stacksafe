#include "tree.hpp"

namespace stacksafe {

Header::Header(Color c, std::size_t k, std::size_t s)
    : rank_{combine(c, k)}, size_{s} {}
std::size_t Header::rank() const {
  return rank_ >> 1;
}
std::size_t Header::size() const {
  return size_;
}
bool Header::is_black() const {
  return rank_ & 1;
}
std::size_t Header::combine(Color c, std::size_t k) {
  return (k << 1) | static_cast<std::size_t>(c);
}

}  // namespace stacksafe
