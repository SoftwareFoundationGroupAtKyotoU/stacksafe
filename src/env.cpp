#include "env.hpp"
#include "register.hpp"
#include "symbol.hpp"

namespace stacksafe {

template <typename Key>
auto Env<Key>::get(const Key& key) -> Val* {
  const auto& self = *this;
  return const_cast<Val*>(self.get(key));
}
template <typename Key>
auto Env<Key>::get(const Key& key) const -> const Val* {
  if (auto it = Base::find(key); it != end()) {
    return &it->second;
  }
  return nullptr;
}
template <typename Key>
void Env<Key>::insert(const Key& key, const Symbol& sym) {
  Val val;
  val.insert(sym);
  insert(key, val);
}
template <typename Key>
void Env<Key>::insert(const Key& key, const Val& val) {
  if (auto p = get(key)) {
    p->insert(val);
  } else {
    Base::try_emplace(key, val);
  }
}

template class Env<Register>;
template class Env<Symbol>;

}  // namespace stacksafe
