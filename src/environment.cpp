#include "environment.hpp"

namespace stacksafe {
  namespace {
    constexpr char nullch = '\0';
  }
  char LocationFactory::outlive_ = nullch;
  char LocationFactory::global_ = nullch;
  char LocationFactory::undefined_ = nullch;
  auto LocationFactory::getLocal() -> Location {
    auto data = std::make_unique<char>(nullch);
    local_.push_back(std::move(data));
    return local_.back().get();
  }
  auto LocationFactory::getOutlive() -> Location {
    return &outlive_;
  }
  auto LocationFactory::getGlobal() -> Location {
    return &global_;
  }
  auto LocationFactory::getUndefined() -> Location {
    return &undefined_;
  }
}
