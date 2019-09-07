#ifndef INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8
#define INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8

#include <string>
#include <vector>

namespace llvm {
class raw_ostream;
}

namespace stacksafe {

class Fabric {
  std::vector<std::string> fabric_;
  std::size_t pos_;

 public:
  Fabric();
  void next();
  std::string& current();
  Fabric& append(const std::string& str);
  Fabric& append(const Fabric& fab);
  Fabric& prepend(const std::string& str);
  Fabric& prepend(const Fabric& fab);
  Fabric& quote(const std::string& open, const std::string& close);
  Fabric& quote();
  Fabric& indent(std::size_t width);
  void print(llvm::raw_ostream& os) const;
};
llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const Fabric& fab);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_D970F1B9_9CD5_4979_89FF_67935A12C1D8
