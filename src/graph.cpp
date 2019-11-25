#include "graph.hpp"
#include <llvm/IR/Function.h>

namespace stacksafe {
namespace {
struct Frame {
  int index, low;
  bool on;
  Frame();
  bool undefined() const;
  bool is_root() const;
  void push(int i);
  void update(int n);
  void pop();
};
Frame::Frame() : index{-1}, low{-1}, on{false} {}
bool Frame::undefined() const {
  return index < 0;
}
bool Frame::is_root() const {
  return index == low;
}
void Frame::push(int i) {
  index = low = i;
  on = true;
}
void Frame::update(int n) {
  if (n < index) {
    index = n;
  }
}
void Frame::pop() {
  on = false;
}
}  // namespace

}  // namespace stacksafe
