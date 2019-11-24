#include "filter.hpp"

namespace stacksafe {

BloomFilter::BloomFilter(std::size_t count) : buf_{count, 0} {
  buf_.shrink_to_fit();
}
void BloomFilter::set(std::size_t once, std::size_t twice, std::size_t nth) {
  const auto index = once + twice * nth;
  const auto i = (index / width) % buf_.size();
  const auto j = index % width;
  buf_[i].set(j);
}
bool BloomFilter::test(std::size_t once, std::size_t twice,
                       std::size_t nth) const {
  const auto index = once + twice * nth;
  const auto i = (index / width) % buf_.size();
  const auto j = index % width;
  return buf_[i][j];
}

}  // namespace stacksafe
