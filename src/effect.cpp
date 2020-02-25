#include "effect.hpp"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <tuple>
#include <vector>

namespace stacksafe {

class EffectLine {
  using Views = std::vector<std::string_view>;
  using Mapsto = std::tuple<Index, Index>;
  using Head = std::optional<std::tuple<std::string_view, Arity>>;
  using Tail = std::optional<std::vector<Mapsto>>;
  std::string_view line_;
  std::string_view name_;
  Arity arity_;
  std::vector<Mapsto> map_;

 public:
  explicit EffectLine(std::string_view line);
  std::string_view name() const;
  std::size_t arity() const;
  const std::vector<Mapsto>& map() const;
  bool parse();

 private:
  static Head parse_head(std::string_view head);
  static Tail parse_tail(Arity arity, const Views& tail);
  static std::optional<Mapsto> parse_mapsto(Arity arity, std::string_view v);
  static Views split(std::string_view v, const char* delim);
  static std::optional<int> to_int(std::string_view v);
  static Index to_index(std::string_view v);
};

EffectLine::EffectLine(std::string_view line) : line_{line}, arity_{0} {}
std::string_view EffectLine::name() const {
  return name_;
}
std::size_t EffectLine::arity() const {
  return arity_.value();
}
auto EffectLine::map() const -> const std::vector<Mapsto>& {
  return map_;
}
bool EffectLine::parse() {
  const auto vec = split(line_, ",");
  if (const auto head = parse_head(vec.front())) {
    const auto arity = std::get<1>(*head);
    const Views views{std::next(vec.begin()), vec.end()};
    if (const auto tail = parse_tail(arity, views)) {
      std::tie(name_, arity_) = *std::move(head);
      map_ = *std::move(tail);
      return true;
    }
  }
  return false;
}
auto EffectLine::parse_head(std::string_view head) -> Head {
  const auto pair = split(head, "/");
  if (pair.size() == 2) {
    const auto name = pair[0];
    const auto arity = to_int(pair[1]);
    if (arity) {
      return std::make_tuple(name, Arity{*arity});
    }
  }
  return std::nullopt;
}
auto EffectLine::parse_tail(Arity arity, const Views& tail) -> Tail {
  std::vector<Mapsto> map;
  for (const auto& e : tail) {
    if (const auto pair = parse_mapsto(arity, e)) {
      map.push_back(*std::move(pair));
    } else {
      return std::nullopt;
    }
  }
  return map;
}
auto EffectLine::parse_mapsto(Arity arity, std::string_view v)
    -> std::optional<Mapsto> {
  const auto pair = split(v, ":");
  if (pair.size() == 2) {
    if (const auto lhs = arity.to_index(pair[0])) {
      if (const auto rhs = arity.to_index(pair[1])) {
        return std::make_tuple(lhs, rhs);
      }
    }
  }
  return std::nullopt;
}
auto EffectLine::split(std::string_view v, const char* delim) -> Views {
  Views views;
  const auto next = [&v, delim]() { return v.find_first_of(delim); };
  for (auto pos = next(); pos != v.npos; pos = next()) {
    views.emplace_back(v.substr(0, pos));
    v.remove_prefix(pos + 1);
  }
  views.emplace_back(v);
  return views;
  ;
}
std::optional<int> EffectLine::to_int(std::string_view v) {
  std::string buf{v};
  const auto b = buf.c_str();
  const auto e = b + buf.size();
  if (b != e) {
    char* p = nullptr;
    const auto val = std::strtol(b, &p, 10);
    if (p == e && 0 <= val && val != LONG_MAX && val <= INT_MAX) {
      return static_cast<int>(val);
    }
  }
  return std::nullopt;
}
Index EffectLine::to_index(std::string_view v) {
  if (v == "g") {
    return Index::GLOBAL;
  } else if (v == "r") {
    return Index::RETURN;
  } else if (const auto i = to_int(v)) {
    return static_cast<Index>(*i);
  } else {
    return Index::OTHERS;
  }
}

Effect::Effect(const EffectLine& line)
    : mat_{line.arity() + 2}, name_{line.name()}, arity_{line.arity()} {
  for (const auto& [lhs, rhs] : line.map()) {
    mat_.set(convert(lhs), convert(rhs));
  }
}
std::optional<Effect> Effect::make(std::string_view v) {
  EffectLine line{v};
  if (line.parse()) {
    return Effect{line};
  }
  return std::nullopt;
}
bool Effect::depends(Index from, Index to) const {
  if (!name_.empty()) {
    const auto n = static_cast<Index>(arity_);
    if (from.is_valid(n) && to.is_valid(n)) {
      return mat_.get(convert(from), convert(to));
    }
  }
  return true;
}
std::size_t Effect::convert(Index index) const {
  assert(index.is_valid(static_cast<Index>(arity_)));
  const auto n = static_cast<int>(arity_);
  const auto i = static_cast<int>(index);
  return n - i - 1;
}

}  // namespace stacksafe
