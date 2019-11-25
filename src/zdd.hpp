#ifndef INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
#define INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402

#include <memory>
#include <set>
#include "value.hpp"

namespace stacksafe {
class Zdd;
using ZddPtr = std::shared_ptr<Zdd>;

class Zdd {
  using Ptrs = std::set<ZddPtr>;
  const Pair label_;
  ZddPtr lo_, hi_;
  Zdd(const Pair& pair, ZddPtr lo, ZddPtr hi);

 public:
  Pair label() const;
  static bool includes(const ZddPtr& lhs, const ZddPtr& rhs);
  static ZddPtr merge(const ZddPtr& lhs, const ZddPtr& rhs);
  static ZddPtr make(const std::set<Pair>& pairs);

 private:
  static bool is_top(const ZddPtr& ptr);
  static bool is_bot(const ZddPtr& ptr);
  static bool includes(const Ptrs& lhs, const ZddPtr& rhs);
  static bool cut(Ptrs& out, const ZddPtr& ptr, const Pair& pair);
  static ZddPtr make(const Pair& pair, ZddPtr lo, ZddPtr hi);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
