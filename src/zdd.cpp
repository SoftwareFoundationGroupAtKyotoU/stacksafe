#include "zdd.hpp"
#include <cassert>

namespace stacksafe {

Zdd::Zdd(const Pair& pair, ZddPtr lo, ZddPtr hi)
    : label_{pair}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
Pair Zdd::label() const {
  return label_;
}
bool Zdd::includes(const ZddPtr& lhs, const ZddPtr& rhs) {
  return includes(Ptrs{lhs}, rhs);
}
ZddPtr Zdd::merge(const ZddPtr& lhs, const ZddPtr& rhs) {
  if (!is_bot(lhs) && !is_bot(rhs)) {
    Pair label = Pair::get_zero();
    ZddPtr lo, hi;
    if (lhs->label_ < rhs->label_) {
      label = rhs->label_;
      lo = merge(lhs, rhs->lo_);
      hi = rhs->hi_;
    } else if (rhs->label_ < lhs->label_) {
      label = lhs->label_;
      lo = merge(lhs->lo_, rhs);
      hi = lhs->hi_;
    } else if (lhs->label_ == rhs->label_) {
      label = lhs->label_;
      lo = merge(lhs->lo_, rhs->lo_);
      hi = merge(lhs->hi_, rhs->hi_);
    }
    return make(label, lo, hi);
  }
  return is_bot(lhs) ? rhs : lhs;
}
ZddPtr Zdd::make(const std::set<Pair>& pairs) {
  static const auto top = Zdd::make(Pair::get_zero(), nullptr, nullptr);
  static const auto bot = nullptr;
  auto root = top;
  for (const auto& pair : pairs) {
    root = make(pair, bot, root);
  }
  return root;
}
bool Zdd::is_terminal(const ZddPtr& ptr) {
  return is_top(ptr) || is_bot(ptr);
}
bool Zdd::is_top(const ZddPtr& ptr) {
  return ptr && ptr->label_ == Pair::get_zero();
}
bool Zdd::is_bot(const ZddPtr& ptr) {
  return !ptr;
}
bool Zdd::equals(const ZddPtr& lhs, const ZddPtr& rhs) {
  if (is_bot(lhs) || is_bot(rhs)) {
    return is_bot(lhs) && is_bot(rhs);
  } else {
    return lhs->label_ == rhs->label_ && equals(lhs->lo_, rhs->lo_) &&
           equals(lhs->hi_, rhs->hi_);
  }
}
bool Zdd::includes(const Ptrs& lhs, const ZddPtr& rhs) {
  if (is_terminal(rhs)) {
    return true;
  }
  Ptrs next;
  bool ok = false;
  for (const auto& ptr : lhs) {
    if (cut(next, ptr, rhs->label_)) {
      ok = true;
    }
  }
  if (ok) {
    return includes(next, rhs->lo_) && includes(next, rhs->hi_);
  } else {
    return false;
  }
}
bool Zdd::cut(Ptrs& out, const ZddPtr& ptr, const Pair& pair) {
  if (is_terminal(ptr)) {
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
ZddPtr Zdd::make(const Pair& pair, ZddPtr lo, ZddPtr hi) {
  return std::shared_ptr<Zdd>{new Zdd{pair, std::move(lo), std::move(hi)}};
}

}  // namespace stacksafe
