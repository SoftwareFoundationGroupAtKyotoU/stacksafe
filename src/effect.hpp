#ifndef INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
#define INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93

#include <optional>
#include <string>
#include <string_view>
#include "matrix.hpp"

namespace stacksafe {
class EffectLine;

class Index {
  friend class Arity;
  int index_;

 public:
  static const Index GLOBAL, RETURN, OTHERS;
  explicit Index(int index);
  bool is_valid(Index arity) const;
  explicit operator int() const;
};
class Arity : private Index {
 public:
  explicit Arity(int arity);
  std::optional<Index> make(int index) const;
};

class Effect {
  Matrix mat_;
  std::string name_;
  std::size_t arity_;
  explicit Effect(const EffectLine& line);

 public:
  static std::optional<Effect> make(std::string_view v);
  bool depends(Index from, Index to) const;

 private:
  std::size_t convert(Index index) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
