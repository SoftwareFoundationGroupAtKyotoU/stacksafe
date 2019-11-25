#include "zdd.hpp"
#include <cassert>

namespace stacksafe {

Zdd::Zdd(const Pair& pair, ZddPtr lo, ZddPtr hi)
    : label_{pair}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
Pair Zdd::label() const {
  return label_;
}
bool Zdd::is_top() const {
  return label_ == Pair::get_negative() && !lo_ && !hi_;
}
bool Zdd::is_bot() const {
  return label_ == Pair::get_zero() && !lo_ && !hi_;
}
int Zdd::compare(const Zdd& lhs, const Zdd& rhs) {
  if (lhs.label_ < rhs.label_) {
    return -1;
  } else if (rhs.label_ < lhs.label_) {
    return 1;
  } else {
    return 0;
  }
}
bool Zdd::equals(const ZddPtr& lhs, const ZddPtr& rhs) {
  if (lhs && rhs) {
    return lhs->label_ == rhs->label_ && equals(lhs->lo_, rhs->lo_) &&
           equals(lhs->hi_, rhs->hi_);
  } else {
    return !lhs && !rhs;
  }
}
ZddPtr Zdd::merge(const ZddPtr& lhs, const ZddPtr& rhs) {
  if (lhs && rhs) {
    if (lhs->is_bot()) {
      return rhs;
    } else if (rhs->is_bot()) {
      return lhs;
    }
    switch (compare(*lhs, *rhs)) {
      case -1:
        return make(rhs->label(), merge(lhs, rhs->lo_), rhs->hi_);
      case 1:
        return make(lhs->label(), merge(lhs->lo_, rhs), lhs->hi_);
      case 0:
        return make(lhs->label(), merge(lhs->lo_, rhs->lo_),
                    merge(lhs->hi_, rhs->hi_));
    }
  }
  assert(!lhs && !rhs);
  return lhs;
}
ZddPtr Zdd::make(const std::set<Pair>& pairs) {
  auto bot = get_bot();
  auto root = get_top();
  for (const auto& pair : pairs) {
    root = make(pair, bot, root);
  }
  return root;
}
ZddPtr Zdd::make(const Pair& pair, ZddPtr lo, ZddPtr hi) {
  return std::shared_ptr<Zdd>{new Zdd{pair, std::move(lo), std::move(hi)}};
}
ZddPtr Zdd::get_top() {
  return Zdd::make(Pair::get_negative(), nullptr, nullptr);
}
ZddPtr Zdd::get_bot() {
  return Zdd::make(Pair::get_zero(), nullptr, nullptr);
}

}  // namespace stacksafe
