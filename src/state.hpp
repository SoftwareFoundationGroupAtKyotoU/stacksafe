#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include <map>
#include <set>
#include <vector>
#include "block.hpp"
#include "cell.hpp"
#include "component.hpp"
#include "nlohmann/json.hpp"

namespace llvm {
class BasicBlock;
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class State : private std::vector<Component> {
  using Super = std::vector<Component>;
  using BB = Blocks::value_type;

 public:
  using Super::begin, Super::end;
  explicit State(const llvm::Function &f);
  void transfer(const Component &c);
  bool is_safe() const;

 private:
  Component &find(BB b);
};

}  // namespace stacksafe

namespace dataflow {
class State : private std::map<Cell, Value> {
  using Super = std::map<Cell, Value>;

 public:
  using Super::begin, Super::end;
  void transfer(const llvm::BasicBlock &b);

 private:
  void update(const Cell &key, const Value &val);
  Value eval(const Value &value) const;
  Value eval(const Cell &cell) const;
};
void to_json(nlohmann::json &j, const State &state);
}  // namespace dataflow

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
