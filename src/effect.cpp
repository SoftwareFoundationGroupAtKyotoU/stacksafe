#include "effect.hpp"
#include <llvm/IR/InstrTypes.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <tuple>
#include <vector>

namespace stacksafe {
namespace {
std::optional<std::size_t> to_size_t(std::string_view v) {
  const std::string buf{v};
  const auto b = buf.c_str();
  const auto e = b + buf.size();
  if (b != e) {
    char* p = nullptr;
    const auto val = std::strtol(b, &p, 10);
    if (p == e && 0 <= val && val != LONG_MAX && val <= INT_MAX) {
      return static_cast<std::size_t>(val);
    }
  }
  return std::nullopt;
}

class EffectLine {
  using Views = std::vector<std::string_view>;
  using Mapsto = std::tuple<Index, Index>;
  using Head = std::optional<std::tuple<std::string_view, Arity>>;
  using Tail = std::optional<Mapsto>;
  std::string_view line_;
  std::string_view name_;
  Arity arity_;
  std::vector<Mapsto> map_;

 public:
  explicit EffectLine(std::string_view line);
  std::string_view name() const;
  const Arity& arity() const;
  const std::vector<Mapsto>& map() const;
  bool parse();

 private:
  static Head parse_head(std::string_view head);
  static Tail parse_tail(const Arity& arity, std::string_view v);
  static Views split(std::string_view v, const char* delim);
};

EffectLine::EffectLine(std::string_view line) : line_{line}, arity_{0} {}
std::string_view EffectLine::name() const {
  return name_;
}
const Arity& EffectLine::arity() const {
  return arity_;
}
auto EffectLine::map() const -> const std::vector<Mapsto>& {
  return map_;
}
bool EffectLine::parse() {
  const auto vec = split(line_, ",");
  if (const auto head = parse_head(vec.front())) {
    const auto& arity = std::get<1>(*head);
    std::vector<Mapsto> map;
    for (const auto& tail : Views{std::next(vec.begin()), vec.end()}) {
      if (const auto pair = parse_tail(arity, tail)) {
        map.push_back(*std::move(pair));
      } else {
        return false;
      }
    }
    std::tie(name_, arity_) = *std::move(head);
    map_ = std::move(map);
    return true;
  }
  return false;
}
auto EffectLine::parse_head(std::string_view head) -> Head {
  const auto pair = split(head, "/");
  if (pair.size() == 2) {
    const auto name = pair[0];
    if (const auto arity = to_size_t(pair[1])) {
      return std::make_tuple(name, Arity{*arity});
    }
  }
  return std::nullopt;
}
auto EffectLine::parse_tail(const Arity& arity, std::string_view v) -> Tail {
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
}
}  // namespace

Effect::Effect() : matrix_{0}, name_{} {}
const std::string& Effect::name() const {
  return name_;
}
const Arity& Effect::arity() const {
  return matrix_.arity();
}
bool Effect::init(std::string_view v) {
  EffectLine line{v};
  if (line.parse()) {
    matrix_.init(line.arity());
    name_ = line.name();
    for (const auto& [lhs, rhs] : line.map()) {
      matrix_.set(lhs, rhs);
    }
    return true;
  }
  return false;
}
bool Effect::depends(Index from, Index to) const {
  if (!name_.empty()) {
    assert(arity().is_valid(from));
    assert(arity().is_valid(to));
    return matrix_.get(from, to);
  }
  return true;
}

Effect EffectMap::get(const llvm::CallBase& call) const {
  if (const auto f = call.getCalledFunction()) {
    if (const auto it = Super::find(f->getName().str()); it != Super::end()) {
      const auto& effect = it->second;
      assert(effect.arity().value() == call.arg_size() &&
             "arity of called function must be the same");
      return effect;
    }
  }
  return Effect{};
}
void EffectMap::load(const std::string& file) {
  std::ifstream input{file};
  std::string buf;
  Effect effect;
  int line = 0;
  while (input.good()) {
    std::getline(input, buf);
    if (effect.init(buf)) {
      Super::try_emplace(effect.name(), effect);
    } else {
      const auto msg = "Failed to parse line # %d in %s\n";
      (llvm::errs() << llvm::format(msg, line, file.c_str())).flush();
    }
    ++line;
  }
}

}  // namespace stacksafe
