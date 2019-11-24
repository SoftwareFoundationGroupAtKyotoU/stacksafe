#include "filter.hpp"
#include "hash.hpp"

namespace stacksafe {

BloomFilter::BloomFilter(std::size_t count) : buf_{count, 0} {
  buf_.shrink_to_fit();
}
void BloomFilter::add(std::size_t hash) {
  const auto twice = llvm::hash_value(hash);
  for (std::size_t i = 0; i < num_of_hash; ++i) {
    set(hash, twice, i);
  }
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
