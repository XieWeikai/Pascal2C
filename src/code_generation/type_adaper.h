#pragma once
#include <string>
#include <unordered_map>

namespace pascal2c {
namespace code_generation {
template <typename Key, typename Tp>
using hashmap = ::std::unordered_map<Key, Tp>;
using string = ::std::string;
enum VarType {
    VOID,
    CHAR,
    STRING,
    INT,
    REAL,
    UNDEFINED,
};

class TypeToolKit {
  public:
    const string SymbolToC(const string &pascal_symbol) const;
    const string VarTypeToString(VarType var_type) const;

  private:
    const hashmap<string, string> PascalToCTypeMap = {
        {"integer", "int"}, {"real", "double"}, {"char", "char"},
        {"string", "char"}, {"boolean", "int"}, {"record", "struct"},
        {":=", "="},        {"=", "=="}};
    const hashmap<VarType, string> VarTypeToStringMap = {
        {VarType::VOID, "void"},     {VarType::CHAR, "char"},
        {VarType::STRING, "string"}, {VarType::INT, "int"},
        {VarType::REAL, "real"},     {VarType::UNDEFINED, "undefined"}};
};
} // namespace code_generation
} // namespace pascal2c