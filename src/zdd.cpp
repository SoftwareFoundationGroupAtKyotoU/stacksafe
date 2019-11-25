#include "zdd.hpp"
#include <cassert>

namespace stacksafe {

Zdd::Zdd(const Pair& pair, ZddPtr lo, ZddPtr hi)
    : label_{pair}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
Pair Zdd::label() const {
  return label_;
}
bool Zdd::is_terminal() const {
  return !lo_ || !hi_;
}
bool Zdd::is_top() const {
  return label_ == Pair::get_negative() && is_terminal();
}
bool Zdd::is_bot() const {
  return label_ == Pair::get_zero() && is_terminal();
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
bool Zdd::equals(const Zdd& lhs, const Zdd& rhs) {
  if (lhs.is_terminal() || rhs.is_terminal()) {
    return lhs.label_ == rhs.label_;
  } else {
    return lhs.label_ == rhs.label_ && equals(*lhs.lo_, *rhs.lo_) &&
           equals(*lhs.hi_, *rhs.hi_);
  }
}
bool Zdd::includes(const ZddPtr& lhs, const ZddPtr& rhs) {
  return !rhs || includes(Ptrs{lhs}, *rhs);
}
bool Zdd::includes(const Ptrs& lhs, const Zdd& rhs) {
  if (rhs.is_terminal()) {
    return true;
  }
  Ptrs next;
  bool ok = false;
  for (const auto& ptr : lhs) {
    if (cut(next, ptr, rhs.label_)) {
      ok = true;
    }
  }
  if (ok) {
    return includes(next, *rhs.lo_) && includes(next, *rhs.hi_);
  } else {
    return false;
  }
}
bool Zdd::cut(Ptrs& out, const ZddPtr& ptr, const Pair& pair) {
  if (!ptr || ptr->is_terminal()) {
    return false;
  }
  if (ptr->label_ < pair) {
    out.insert(ptr);
    return false;
  } else {
    auto lo = cut(out, ptr->lo_, pair);
    auto hi = cut(out, ptr->hi_, pair);
    return (lo || hi || ptr->label_ == pair);
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
