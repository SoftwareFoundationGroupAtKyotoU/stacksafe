#include "value.hpp"
#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Value::prefix_{"%"};
Value::Value(int n, llvm::Type *t, const llvm::Value *v)
    : Token{n, Type{t}}, value_{v} {}
Value Value::make(const llvm::Value &v) {
  if (llvm::isa<llvm::Constant>(v)) {
    auto tag = llvm::isa<llvm::ConstantAggregate>(v) ?
                   "aggregate" :
                   llvm::isa<llvm::ConstantData>(v) ?
                   "data" :
                   llvm::isa<llvm::ConstantExpr>(v) ?
                   "expr" :
                   llvm::isa<llvm::GlobalValue>(v) ? "global" : "constant";
    llvm::errs() << tag << ": " << v << "\n";
  }
  int num = -1;
  auto operand = get_operand(v);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix_) {
    if (auto i = to_int(view.substr(1))) {
      num = *i;
    }
  }
  return Value{num, v.getType(), &v};
}
Value::Value(int n) : Value{n, nullptr, nullptr} {}
const llvm::Value *Value::get() const { return value_; }
std::string Value::repr() const {
  if (is_register()) {
    return prefix_ + std::to_string(number());
  } else {
    return to_str(*value_);
  }
}
bool Value::is_register() const { return 0 <= number(); }
bool operator<(const Value &lhs, const Value &rhs) {
  if (lhs.is_register() || rhs.is_register()) {
    return lhs.number() < rhs.number();
  } else {
    return lhs.repr() < rhs.repr();
  }
}
void to_json(Json &j, const Value &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Value &value) {
  Fabric ret;
  return ret.append(value.repr()).quote();
}

}  // namespace stacksafe
