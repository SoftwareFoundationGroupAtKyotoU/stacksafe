#include "symbol.hpp"

int Symbol::current_ = 0;
Symbol::Symbol(int n) : num_{n} {}
Symbol Symbol::create() { return Symbol{current_++}; }
