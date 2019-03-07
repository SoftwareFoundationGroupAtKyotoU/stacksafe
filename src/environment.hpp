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
}
namespace stacksafe {
  using LocationSet = Set<Location>;
  using Heap = Map<Location, Location>;
  using Stack = Map<Register, Location>;

  class Env {
    Heap heap_;
    Stack stack_;
    LocationFactory &factory_;
  public:
    explicit Env(LocationFactory &factory);
    bool subsetof(const Env &rhs) const;
    void unify(const Env &rhs);
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
  private:
    std::optional<LocationSet> to_symbols(const Value &v) const;
    static bool just_added(bool cond);
    friend class Reachable;
  };

  class Reachable {
    Set<Register> args_;
    LocationSet reachable_;
    Heap &heap_;
    Stack &stack_;
  public:
    explicit Reachable(Env &env);
    bool add(const Register &reg);
  private:
    bool add(const LocationSet &locs);
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
