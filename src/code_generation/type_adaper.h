#pragma once
#include <string>
#include <unordered_map>

namespace pascal2c {
namespace code_generation {
template <typename Key, typename Tp>
using hashmap = ::std::unordered_map<Key, Tp>;
using string = ::std::string;

class TypeToolKit {
  public:
    const string TypeToC(const string &pascal_type) const;

  private:
    const hashmap<string, string> PascalToCTypeMap = {
        {"integer", "int"}, {"real", "double"}, {"character", "char"},
        {"string", "char"}, {"boolean", "int"}, {"record", "struct"},
        {":=", "="},
    };
};
} // namespace code_generation
} // namespace pascal2c