#include "../ast/ast.h"

namespace symbol_table{
    //item of symbol table
    class SymbolTableItem{
    public:
        SymbolTableItem(std::string name, pascal2c::ast::ExprType type) 
            : name_(name), type_(type){};
        std::string Name(){return name_;}
        pascal2c::ast::ExprType Type(){return type_;}
    private:
        std::string name_;
        pascal2c::ast::ExprType type_;
    };
	
    class SymbolTableBlock{
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
        
    private:
        std::shared_ptr<SymbolTableBlock> father;
        map<string,pascal2c::ast::ExprType> idlist;
        
    };
    
	class SymbolTable{
    public:
        //create a new block of SymbolTable from father block
        void Locate(std::shared_ptr<SymbolTableBlock> father);
        
        //delete the block of SymbolTable and return to father block
        std::shared_ptr<SymbolTableBlock> Relocate(std::shared_ptr<SymbolTableBlock> to_del);
    private:
    };
    
}
