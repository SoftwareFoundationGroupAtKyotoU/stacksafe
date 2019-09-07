#include "token.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <optional>
#include <string_view>
#include "json.hpp"

namespace stacksafe {
namespace {
std::optional<int> to_int(std::string_view view) {
  if (!view.empty()) {
    std::string str{view};
    std::size_t pos = std::string_view::npos;
    auto val = std::stoi(str, &pos, 10);
    if (pos == view.size()) {
      return val;
    }
  }
  return std::nullopt;
}
std::string to_alphabet(int num) {
  std::string ret;
  for (; 0 < num; num /= 26) {
    ret.insert(0, 1, 'a' + --num % 26);
  }
  return ret;
}
std::string to_str(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  stream << value;
  return stream.str();
}
std::string get_operand(const llvm::Value &value) {
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  value.printAsOperand(stream, false);
  return stream.str();
}
int extract_num(const llvm::Value &value, std::string_view prefix) {
  auto operand = get_operand(value);
  std::string_view view{operand};
  if (!view.empty() && view.substr(0, 1) == prefix) {
    return to_int(view.substr(1)).value_or(-1);
  }
  return -1;
}
}  // namespace

Token::Token(int n, const Type &t) : num_{n}, type_{t} {}
int Token::number() const { return num_; }
const Type &Token::type() const { return type_; }

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = Symbol::current_init;
Symbol Symbol::global(const Type &t) { return Symbol{current_init, t}; }
Symbol Symbol::create(const Type &t) { return Symbol{++current_, t}; }
void Symbol::reset() { current_ = current_init; }
std::string Symbol::repr() const {
  if (is_global()) {
    return prefix_ + "*";
  } else {
    return prefix_ + to_alphabet(number());
  }
}
bool Symbol::is_global() const { return number() <= current_init; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }

const std::string Value::prefix_{"%"};
Value::Value(int n, llvm::Type *t, const llvm::Value *v)
    : Token{n, Type{t}}, value_{v} {}
Value Value::create(const llvm::Value &v) {
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
void to_json(Json &j, const Value &x) { j = x.repr(); }

}  // namespace stacksafe
