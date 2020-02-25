#include "effect.hpp"
#include <climits>
#include <cstdlib>
#include <tuple>
#include <vector>

namespace stacksafe {
class EffectLine {
  using Views = std::vector<std::string_view>;
  using Mapsto = std::tuple<std::size_t, std::size_t>;
  using Head = std::optional<std::tuple<std::string_view, std::size_t>>;
  using Tail = std::optional<std::vector<Mapsto>>;
  std::string_view line_;
  std::string_view name_;
  std::size_t arity_;
  std::vector<Mapsto> map_;

 public:
  explicit EffectLine(std::string_view line);
  std::string_view name() const;
  std::size_t arity() const;
  const std::vector<Mapsto>& map() const;
  bool parse();

 private:
  static Head parse_head(std::string_view head);
  static Tail parse_tail(const Views& tail);
  static Views split(std::string_view v, const char* delim);
  static std::optional<std::size_t> to_size_t(std::string_view v);
  static std::optional<int> to_int(std::string_view v);
};

EffectLine::EffectLine(std::string_view line) : line_{line}, arity_{0} {}
std::string_view EffectLine::name() const {
  return name_;
}
std::size_t EffectLine::arity() const {
  return arity_;
}
auto EffectLine::map() const -> const std::vector<Mapsto>& {
  return map_;
}
bool EffectLine::parse() {
  const auto vec = split(line_, ",");
  if (const auto head = parse_head(vec.front())) {
    const Views views{std::next(vec.begin()), vec.end()};
    if (const auto tail = parse_tail(views)) {
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
    if (const auto arity = to_size_t(pair[1])) {
      return std::make_tuple(pair[0], *arity);
    }
  }
  return std::nullopt;
}
auto EffectLine::parse_tail(const Views& tail) -> Tail {
  std::vector<Mapsto> map;
  for (const auto& e : tail) {
    const auto pair = split(e, ":");
    if (pair.size() != 2) {
      return std::nullopt;
    }
    const auto lhs = to_size_t(pair[0]);
    const auto rhs = to_size_t(pair[1]);
    if (lhs && rhs) {
      map.emplace_back(*lhs, *rhs);
    } else {
      return std::nullopt;
    }
  }
  return map;
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
std::optional<std::size_t> EffectLine::to_size_t(std::string_view v) {
  std::string buf{v};
  const auto b = buf.c_str();
  const auto e = b + buf.size();
  if (b != e) {
    char* p = nullptr;
    const auto val = std::strtol(b, &p, 10);
    if (p == e && 0 <= val && val != LONG_MAX) {
      return val;
    }
  }
  return std::nullopt;
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

Effect::Effect(const EffectLine& line)
    : mat_{line.arity() + 2}, name_{line.name()}, arity_{line.arity()} {
  for (const auto& [lhs, rhs] : line.map()) {
    mat_.set(lhs, rhs);
  }
}
std::optional<Effect> Effect::make(std::string_view v) {
  EffectLine line{v};
  if (line.parse()) {
    return Effect{line};
  }
  return std::nullopt;
}

}  // namespace stacksafe
