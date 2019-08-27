#include "abstract.hpp"
#include "interpret.hpp"
#include "nlohmann/json.hpp"

namespace stacksafe {
namespace {
nlohmann::json to_json(const std::map<Register, Symbol>& stack) {
  nlohmann::json j;
  for (auto& [k, v] : stack) {
    j[k.to_str()] = v.to_str();
  }
  return j;
}
nlohmann::json to_json(const std::map<Symbol, Symbol>& heap) {
  nlohmann::json j;
  for (auto& [k, v] : heap) {
    j[k.to_str()] = v.to_str();
  }
  return j;
}
}  // namespace

void Abstraction::interpret(llvm::Function& f) {
  Interpret interpreter(*this);
  interpreter.visit(f);
}
void Abstraction::argument(const Register& arg) {
  stack_.try_emplace(arg, Symbol::create());
}
void Abstraction::alloc(const Register& r) {
  stack_.try_emplace(r, Symbol::create());
}
void Abstraction::store(const Register& src, const Register& dst) {
  auto source = stack_.find(src);
  auto target = stack_.find(dst);
  auto end = stack_.end();
  if (source != end && target != end) {
    heap_.try_emplace(target->second, source->second);
  }
}
void Abstraction::show() const {
  nlohmann::json j;
  j["stack"] = to_json(stack_);
  j["heap"] = to_json(heap_);
  llvm::errs() << j.dump(2) << "\n";
}

}  // namespace stacksafe
