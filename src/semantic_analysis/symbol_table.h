#include "../ast/ast.h"

namespace symbol_table{
    //item of symbol table
    class SymbolTableItem{
    public:
        SymbolTableItem(std::string name, pascal2c::ast::ExprType type) 
            : name_(name), type_(type){};
    private:
        std::string name_;
        pascal2c::ast::ExprType type_;
    };
    
    class SymbolTable{
    public:
        //add identify: id_name=name id_type=type
        //return success or failure
        //e.g. a := 2 lead to add("a",int);
        bool AddItem(std::string name, pascal2c::ast::ExprType type);
        //find identify id_name=name
        //return success or failure  if success,ansType=id_type
        //e.g. a := 2 
        //     Query("a",a_type); -> return true,a_type=ExprType.INT
        bool Query(std::string name,pascal2c::ast::ExprType &ansType);
        //create a new block of SymbolTable
        void Locate();
        //delete the latest block of SymbolTable
        void Relocate();
    private:
        
    };
}
