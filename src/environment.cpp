#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
namespace stacksafe {
  Location::Location(const char &c)
    : loc_(&c)
  {}
  size_t Location::hash() const {
    return Base::operator()(loc_);
  }
  bool Location::operator<(const Location &rhs) const {
    return loc_ < rhs.loc_;
  }
  bool Location::operator==(const Location &rhs) const {
    return loc_ == rhs.loc_;
  }

  namespace {
    constexpr char nullch = '\0';
  }
  char LocationFactory::outlive_ = nullch;
  char LocationFactory::global_ = nullch;
  char LocationFactory::undefined_ = nullch;
  Location LocationFactory::getLocal() {
    local_.push_back(std::make_unique<char>(nullch));
    return static_cast<Location>(*local_.back());
  }
  Location LocationFactory::getOutlive() {
    return static_cast<Location>(outlive_);
  }
  Location LocationFactory::getGlobal() {
    return static_cast<Location>(global_);
  }
  Location LocationFactory::getUndefined() {
    return static_cast<Location>(undefined_);
  }

  bool AbstractType::subsetof(const AbstractType &rhs) const {
    auto &r = rhs.set_;
    auto &l = set_;
    return std::includes(begin(r), end(r), begin(l), end(l));
  }

  bool HeapMap::subsetof(const HeapMap &rhs) const {
    for (auto &e : map_) {
      auto k = std::get<0>(e);
      auto &l = std::get<1>(e);
      if (auto r = rhs.get(k)) {
        if (l.subsetof(*r)) {
          continue;
        }
      }
      return false;
    }
    return true;
  }
  auto HeapMap::get(Key key) const -> Maybe {
    auto it = map_.find(key);
    if (it == end(map_)) {
      return std::nullopt;
    } else {
      return std::cref(std::get<1>(*it));
    }
  }

  bool EnvMap::subsetof(const EnvMap &rhs) const {
    for (auto &e : map_) {
      auto k = std::get<0>(e);
      auto &l = std::get<1>(e);
      if (auto r = rhs.get(k)) {
        if (l.subsetof(*r)) {
          continue;
        }
      }
      return false;
    }
    return true;
  }
  auto EnvMap::get(Key key) const -> Maybe {
    auto it = map_.find(key);
    if (it == end(map_)) {
      return std::nullopt;
    } else {
      return std::cref(std::get<1>(*it));
    }
  }
}
