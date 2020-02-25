#include "effect.hpp"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <tuple>
#include <vector>

namespace stacksafe {
namespace {
std::optional<int> to_int(std::string_view v) {
  const std::string buf{v};
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
}  // namespace

class EffectLine {
  using Views = std::vector<std::string_view>;
  using Mapsto = std::tuple<Index, Index>;
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
  static Tail parse_tail(Arity arity, const Views& tail);
  static Views split(std::string_view v, const char* delim);
  static std::optional<int> to_int(std::string_view v);
  static Index to_index(std::string_view v);
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
    const Arity arity{static_cast<int>(std::get<1>(*head))};
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
      return std::make_tuple(name, *arity);
    }
  }
  return std::nullopt;
}
auto EffectLine::parse_tail(Arity arity, const Views& tail) -> Tail {
  std::vector<Mapsto> map;
  for (const auto& e : tail) {
    const auto pair = split(e, ":");
    if (pair.size() != 2) {
      return std::nullopt;
    }
    const auto lhs = arity.to_index(pair[0]);
    const auto rhs = arity.to_index(pair[1]);
    if (lhs && rhs) {
      map.emplace_back(lhs, rhs);
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

const Index Index::GLOBAL{-1}, Index::RETURN{-2}, Index::OTHERS{-3};
Index::Index(int index) : index_{index} {
  assert(0 <= index);
}
bool Index::is_valid(Index arity) const {
  return OTHERS.index_ < index_ && index_ < arity.index_;
}
Index::operator int() const {
  return index_;
}
Index::operator bool() const {
  return index_ != OTHERS.index_;
}
Arity::Arity(int arity) : Index{arity} {}
Index Arity::index(int i) const {
  if (OTHERS.index_ < i && i < this->index_) {
    return Index{i};
  }
  return Index::OTHERS;
}
Index Arity::to_index(std::string_view v) const {
  if (v == "g") {
    return GLOBAL;
  } else if (v == "r") {
    return RETURN;
  } else if (const auto i = to_int(v);
             i && OTHERS.index_ < *i && *i < this->index_) {
    return Index{*i};
  } else {
    return OTHERS;
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
