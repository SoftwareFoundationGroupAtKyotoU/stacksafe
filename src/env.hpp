#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <vector>
#include "json_fwd.hpp"
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Fabric;
class Symbol;
class Value;
using Params = std::vector<Value>;

class Env {
  Heap heap_;
  Stack stack_;
  friend void to_json(Json& j, const Env& x);
  friend Fabric dump(const Env& env);

 public:
  Env() = default;
  explicit Env(llvm::Function& f);
  bool merge(const Env& that);
  void argument(const Value& arg);
  void alloc(const Value& dst);
  void store(const Value& src, const Value& dst);
  void load(const Value& dst, const Value& src);
  void constant(const Value& dst);
  void call(const Value& dst, const Params& params);
  void cast(const Value& dst, const Value& src);

 private:
  void collect(const Symbol& symbol, Domain& done) const;
  Domain collect(const Params& value) const;
};
void to_json(Json& j, const Env& x);
Fabric dump(const Env& env);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
