#ifndef INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
#define INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402

#include <memory>
#include <set>
#include "value.hpp"

namespace stacksafe {
class Zdd;
using ZddPtr = std::shared_ptr<Zdd>;

class Zdd {
  const Pair label_;
  ZddPtr lo_, hi_;
  Zdd(const Pair& pair, ZddPtr lo, ZddPtr hi);

 public:
  Pair label() const;
  bool is_terminal() const;
  bool is_top() const;
  bool is_bot() const;
  static int compare(const Zdd& lhs, const Zdd& rhs);
  static bool equals(const ZddPtr& lhs, const ZddPtr& rhs);
  static ZddPtr merge(const ZddPtr& lhs, const ZddPtr& rhs);
  static ZddPtr make(const std::set<Pair>& pairs);
  static ZddPtr make(const Pair& pair, ZddPtr lo, ZddPtr hi);
  static ZddPtr get_top();
  static ZddPtr get_bot();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
