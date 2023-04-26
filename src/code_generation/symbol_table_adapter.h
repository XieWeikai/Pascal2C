#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace pascal2c {
namespace code_generation {
using ::std::shared_ptr;
using ::std::string;
using ::std::unordered_map;

class Row {
  public:
    Row(string name, bool is_reference = false)
        : name_(name), is_reference_(is_reference) {}
    bool IsReference() const { return is_reference_; }

  private:
    string name_;
    bool is_reference_;
};

class SymbolTable {
  public:
    SymbolTable() {}
    void AddVariable(const string &name, bool is_reference);
    bool IsReference(const string &name) const;

  private:
    unordered_map<string, shared_ptr<Row>> table_;
};
} // namespace code_generation
} // namespace pascal2c