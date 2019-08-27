#include "abstract.hpp"
#include "interpret.hpp"
#include "nlohmann/json.hpp"

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
