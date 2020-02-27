#include "dataflow.hpp"
#include "block.hpp"
#include "pointsto.hpp"
#include "tarjan.hpp"

namespace stacksafe {

DataFlow::DataFlow(const llvm::Function& f) : func_{f} {}
void DataFlow::analyze() {
  for (const auto& [b, m] : Tarjan::run(func_)) {
    state_.emplace_back(b);
  }
  for (auto&& c : state_) {
    PointsTo::analyze(c.graph(), c.blocks());
    state_.transfer(c);
  }
}

}  // namespace stacksafe
