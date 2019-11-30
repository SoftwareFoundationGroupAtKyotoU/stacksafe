#include "depend.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {
namespace {
using StringViewPair = std::tuple<std::string_view, std::string_view>;
StringViewPair split(std::string_view v, const char* delim) {
  if (const auto pos = v.find_first_of(delim); pos != v.npos) {
    return {v.substr(0, pos), v.substr(pos + 1)};
  } else {
    return {v, v.substr(v.size())};
  }
}
std::vector<std::string_view> split_vec(std::string_view v, const char* delim) {
  std::vector<std::string_view> ret;
  while (true) {
    if (const auto pos = v.find_first_of(delim); pos != v.npos) {
      ret.emplace_back(v.substr(0, pos));
      v.remove_prefix(pos + 1);
    } else {
      ret.emplace_back(v);
      break;
    }
  }
  return ret;
}
std::optional<std::size_t> to_size_t(std::string_view v) {
  std::string buf{v};
  char* ptr = nullptr;
  const auto val = std::strtol(buf.c_str(), &ptr, 10);
  if (ptr != buf.c_str() && 0 <= val && val != LONG_MAX) {
    return val;
  }
  return std::nullopt;
}
}  // namespace

Matrix::Matrix(std::size_t n) : Super(n * n, 0), size_{n} {}
std::size_t Matrix::size() const {
  return size_;
}
void Matrix::set(std::size_t row, std::size_t col) {
  assert(row < size_ && col < size_);
  Super::at(row * size_ + col) = 1;
}
bool Matrix::get(std::size_t row, std::size_t col) const {
  assert(row < size_ && col < size_);
  return Super::at(row * size_ + col) != 0;
}

Depend::Depend(std::size_t arity, const std::string& name)
    : Matrix{arity + 2}, name_{name} {}
Depend::Depend(const llvm::Function& f)
    : Depend{f.arg_size(), f.getName().str()} {}
void Depend::set(std::string_view pair) {
  const auto [head, tail] = split(pair, ":");
  if (const auto from = to_index(head)) {
    if (const auto to = to_index(tail)) {
      Matrix::set(*from, *to);
    }
  }
}
void Depend::set(const Symbol& key, const Symbol& val) {
  const auto to = to_index(key);
  const auto from = to_index(val);
  set(from, to);
}
void Depend::set_return(const Symbol& sym) {
  const auto from = to_index(sym);
  const auto to = local_index();
  Matrix::set(from, to);
}
bool Depend::is_error() const {
  for (auto to = 0_z; to < size(); ++to) {
    if (get_error(to)) {
      return true;
    }
  }
  return false;
}
void Depend::print(llvm::raw_ostream& os) const {
  static const auto comma = ",";
  static const auto colon = ":";
  if (is_error()) {
    return;
  }
  for (auto from = 0_z; from < size(); ++from) {
    for (auto to = 0_z; to < size(); ++to) {
      if (get(from, to)) {
        os << comma << to_str(from) << colon << to_str(to);
      }
    }
  }
}
void Depend::print_error(llvm::raw_ostream& os) const {
  for (auto to = 0_z; to < global_index(); ++to) {
    if (get_error(to)) {
      os << "ERROR: ARGUMENT\n";
      break;
    }
  }
  if (get_error(global_index())) {
    os << "ERROR: GLOBAL\n";
  }
  if (get_error(local_index())) {
    os << "ERROR: RETURN\n";
  }
  os.flush();
}
std::size_t Depend::arity() const {
  return size() - 2;
}
std::size_t Depend::local_index() const {
  return arity() + 1;
}
std::size_t Depend::global_index() const {
  return arity();
}
void Depend::set(std::size_t from, std::size_t to) {
  if (from != to && to < local_index()) {
    Matrix::set(from, to);
  }
}
bool Depend::get(std::size_t from, std::size_t to) const {
  if (from != to && from < local_index()) {
    return Matrix::get(from, to);
  }
  return false;
}
bool Depend::get_error(std::size_t to) const {
  return Matrix::get(local_index(), to);
}
std::size_t Depend::to_index(const Symbol& sym) const {
  if (sym.is_local()) {
    return local_index();
  } else if (sym.is_global()) {
    return global_index();
  } else {
    auto arg = sym.as_argument();
    assert(arg && "invalid symbol");
    return arg->getArgNo();
  }
}
std::optional<std::size_t> Depend::to_index(std::string_view v) const {
  if (v == "r") {
    return local_index();
  } else if (v == "g") {
    return global_index();
  } else if (auto val = to_size_t(v); *val && *val < global_index()) {
    return *val < global_index();
  }
  return std::nullopt;
}
std::string Depend::to_str(std::size_t i) const {
  if (local_index() == i) {
    return "r";
  } else if (global_index() == i) {
    return "g";
  } else {
    return std::to_string(i);
  }
}

DependMap::DependMap() {
  static const auto name = "depend.txt";
  std::ifstream input{name};
  std::string buf;
  while (input.good()) {
    std::getline(input, buf);
    load(buf);
  }
}
Depend* DependMap::init(std::string_view header) {
  const auto [key, arity] = split(header, "/");
  if (const auto num = to_size_t(arity)) {
    std::string name{key};
    const auto [it, update] = Super::try_emplace(name, *num, name);
    if (update) {
      return &it->second;
    } else {
      std::string msg{"WARNING: depend entry exists: "};
      msg.append(key);
      debug::print(msg);
    }
  }
  return nullptr;
}
void DependMap::load(std::string_view line) {
  if (auto csv = split_vec(line, ","); !csv.empty()) {
    if (auto ptr = init(csv[0])) {
      csv.erase(csv.begin());
      for (const auto& pair : csv) {
        ptr->set(pair);
      }
    }
  }
}

}  // namespace stacksafe
