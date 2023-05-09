#pragma once
#include "ast_adapter.h"
#include <string>
namespace pascal2c {
namespace code_generation {
using ::std::string;

class SymbolItem {
  public:
    SymbolItem(const string &name, bool is_reference = false,
               bool is_return_var = false)
        : name_(name), is_reference_(is_reference),
          is_return_var_(is_return_var) {}
    bool IsReference() const { return is_reference_; }
    bool IsReturnVar() const { return is_return_var_; }

  private:
    string name_;
    bool is_reference_;
    bool is_return_var_;
};

} // namespace code_generation
} // namespace pascal2c