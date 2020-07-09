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
class ValueSet : private std::set<Cell> {
  using Super = std::set<Cell>;

 public:
  using Super::begin, Super::end, Super::insert;
  void insert(const ValueSet &set);
};
void to_json(nlohmann::json &j, const llvm::Value *v);
void to_json(nlohmann::json &j, const ValueSet &set);

class State : private std::map<Cell, ValueSet> {
  using Super = std::map<Cell, ValueSet>;

 public:
  using Super::begin, Super::end;
  ValueSet eval(const llvm::Value *v) const;
  ValueSet eval(const Cell &cell) const;
  void update(const Cell &key, const ValueSet &val);
  void transfer(const llvm::BasicBlock &b);
};
void to_json(nlohmann::json &j, const State &state);
}  // namespace dataflow

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
