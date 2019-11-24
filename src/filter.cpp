#include "filter.hpp"

namespace stacksafe {

BloomFilter::BloomFilter(std::size_t count) : buf_{count, 0} {
  buf_.shrink_to_fit();
}

}  // namespace stacksafe
