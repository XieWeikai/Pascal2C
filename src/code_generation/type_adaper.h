#pragma once
#include <string>
#include <unordered_map>

namespace pascal2c {
namespace code_generation {
template <typename Key, typename Tp>
using hashmap = ::std::unordered_map<Key, Tp>;
using string = ::std::string;
/**
 * @brief Sort by type priority
 * boolean < char < integer < real
*/
enum VarType {
    BOOL    ,
    CHAR    ,
    INT     ,
    REAL    ,
    STRING  ,
    VOID    ,
    UNDEFINED,
};

class TypeToolKit {
  public:
    const string SymbolToC(const string &pascal_symbol) const;
    const string VarTypeToString(VarType var_type) const {
        return VarTypeToStringMap.at(var_type);
    }
    VarType StringToVarType(const string &s) const {
        return StringToVarTypeMap.at(s);
    }
    VarType MergeType(VarType a , VarType b , const string& op) const;

  private:
    const hashmap<string, string> PascalToCTypeMap = {
        {"integer", "int"}, {"real", "double"}, {"char", "char"},
        {"string", "char"}, {"boolean", "int"}, {"record", "struct"},
        {":=", "="},        {"=", "=="},        {"mod", "%"},
        {"div", "/"},       {"/", "/"}};
    const hashmap<VarType, string> VarTypeToStringMap = {
        {VarType::VOID, "void"},          {VarType::CHAR, "char"},
        {VarType::STRING, "string"},      {VarType::INT, "int"},
        {VarType::REAL, "real"},          {VarType::BOOL, "bool"},
        {VarType::UNDEFINED, "undefined"}};
    const hashmap<string, VarType> StringToVarTypeMap = {
        {"void", VarType::VOID},
        {"char", VarType::CHAR},
        {"string", VarType::STRING},
        {"int", VarType::INT},
        {"real", VarType::REAL},
        {"double", VarType::REAL},
        {"bool", VarType::BOOL},
        {"boolean", VarType::BOOL},
        {"undefined", VarType ::UNDEFINED},
    };
};
} // namespace code_generation
} // namespace pascal2c