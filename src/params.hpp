#ifndef INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
#define INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953

#include <map>
#include <vector>
#include "graph.hpp"
#include "matrix.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class Params : private std::vector<const llvm::Value *> {
  using Super = std::vector<const llvm::Value *>;
  class ParamsIterator;

 public:
  using Super::size;
  ParamsIterator begin() const;
  ParamsIterator end() const;
  void push_back(const llvm::Value &v);
};

class Params::ParamsIterator : private Params::Super::const_iterator {
  friend class Params;
  using It = Super::const_iterator;
  explicit ParamsIterator(It it);
  It &super();
  const It &super() const;

 public:
  using It::iterator_category, It::value_type, It::difference_type, It::pointer,
      It::reference;
  const llvm::Value &operator*() const;
  ParamsIterator &operator++();
  bool operator==(ParamsIterator it) const;
  bool operator!=(ParamsIterator it) const;
};

class NodeMap : std::map<Index, NodeSet> {
  using Super = std::map<Index, NodeSet>;

 public:
  using Super::begin, Super::end;
  NodeMap(const Params &params, Graph &graph);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_7033D3E3_92CE_493B_862B_265AB050E953
