#include "params.hpp"
#include "graph.hpp"

namespace stacksafe {

auto Params::begin() const -> ParamsIterator {
  return ParamsIterator{Super::begin()};
}
auto Params::end() const -> ParamsIterator {
  return ParamsIterator{Super::end()};
}
void Params::push_back(const llvm::Value &v) {
  Super::push_back(&v);
}

Params::ParamsIterator::ParamsIterator(It it) : It{it} {}
auto Params::ParamsIterator::super() -> It & {
  return *this;
}
auto Params::ParamsIterator::super() const -> const It & {
  return *this;
}
const llvm::Value &Params::ParamsIterator::operator*() const {
  return **super();
}
auto Params::ParamsIterator::operator++() -> ParamsIterator & {
  ++super();
  return *this;
}
bool Params::ParamsIterator::operator==(ParamsIterator it) const {
  return super() == it.super();
}
bool Params::ParamsIterator::operator!=(ParamsIterator it) const {
  return super() != it.super();
}

NodeMap::NodeMap(const Params &params, Graph &graph) {
  auto &self = *this;
  auto index = Index::GLOBAL;
  graph.reachables(Node::get_global(), self[index]);
  for (const auto &arg : params) {
    graph.reachables(arg, self[++index]);
  }
}

}  // namespace stacksafe
