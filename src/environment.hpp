#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include "collection.hpp"
#include "location.hpp"
#include "register.hpp"
#include "visualize.hpp"
#include <functional>
#include <memory>

namespace llvm {
class raw_ostream;
} // namespace llvm

namespace stacksafe {
using LocationSet = Set<Location>;
using Heap = Env<Location, Location>;
using Stack = Env<Register, Location>;

class Env_ {
  using UserRange = llvm::User::const_op_range;
  Heap heap_;
  Stack stack_;
  LocationFactory &factory_;

public:
  explicit Env_(LocationFactory &factory);
  bool subsetof(const Env_ &rhs) const;
  void unify(const Env_ &rhs);
  void print(llvm::raw_ostream &O) const;

public:
  bool init_argument(const Register &dst);
  bool alloca(const Register &dst);
  bool store(const Value &src, const Register &dst);
  bool load(const Register &dst, const Register &src);
  bool getelementptr(const Register &dst, const Register &src);
  bool binary(const Register &dst);
  bool cast(const Register &dst, const Value &src);
  bool phi(const Register &dst, const Value &src);
  bool cmp(const Register &dst);
  bool call(const UserRange &args, std::optional<Register> dst);

private:
  std::optional<LocationSet> to_symbols(const Value &v) const;
  bool reach(const LocationSet &locs, LocationSet &reachs) const;
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
