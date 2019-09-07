#ifndef INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8
#define INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8

#include <string>
#include <vector>

namespace stacksafe {

class Fabric {
  std::vector<std::string> fabric_;
  std::size_t pos_;

 public:
  Fabric();
  void next();
  std::string& current();
  Fabric& append(const std::string& str);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8
