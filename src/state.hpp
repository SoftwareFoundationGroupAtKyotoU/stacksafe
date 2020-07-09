#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include <map>
#include <set>
#include <vector>
#include "block.hpp"
#include "component.hpp"

namespace llvm {
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
class ValueSet : private std::set<const llvm::Value *> {
  using Super = std::set<const llvm::Value *>;

 public:
  using Super::begin, Super::end, Super::insert;
  void insert(const ValueSet &set);
};
class State : private std::map<const llvm::Value *, ValueSet> {
  using Super = std::map<const llvm::Value *, ValueSet>;

 public:
  ValueSet eval(const llvm::Value *v) const;
};
}  // namespace dataflow

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
