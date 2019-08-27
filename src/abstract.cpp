#include "abstract.hpp"
#include "interpret.hpp"
#include "nlohmann/json.hpp"

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
void Abstraction::alloc(const Register& r) {
  stack_.try_emplace(r, Symbol::create());
}
void Abstraction::show() const {
  using json = nlohmann::json;
  json j;
  for (auto& [k, v] : env_) {
    j[k.to_str()] = v.to_str();
  }
  llvm::errs() << j.dump(2) << "\n";
}
