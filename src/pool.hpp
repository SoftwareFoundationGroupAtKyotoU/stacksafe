#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class FlatEnv;
class FlatMap;
class FlatMapRef;
class MultiMap;

class FlatMapPtr : private std::unique_ptr<MultiMap> {
  friend class FlatMapPool;
  using Super = std::unique_ptr<MultiMap>;
  explicit FlatMapPtr(const FlatMap& flat);

 public:
  FlatMapPtr(FlatMapPtr&&);
  ~FlatMapPtr();
  FlatMapPtr& operator=(FlatMapPtr&&);
  const MultiMap& get() const;
};
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs);

class FlatMapPool : private std::vector<FlatMapPtr> {
  using Super = std::vector<FlatMapPtr>;

 public:
  FlatMapRef add(const FlatMap& flat);
  Env add(const FlatEnv& env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
