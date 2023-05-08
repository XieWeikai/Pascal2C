#ifndef TEST_H
#define TEST_H
#include "symbol_table.h"
#include "../ast/program.h"
#include "errors.h"
namespace analysiser{
    //manage SymbolTableBlock
    class nameTable{
    public:
        //add new SymbolTableBlock with name
        //return success or failure
        bool Add(std::string name);
        //find SymbolTableBlock named name
        //return success or failure. if success, put target on ansblock
        bool Query(std::string name, std::shared_ptr<symbol_table::SymbolTableBlock> &ansblock);
    private:
        std::map<std::string, std::shared_ptr<symbol_table::SymbolTableBlock> > table_;
   };
    
    std::vector<std::string> GetErrors();
    saERRORS::ERROR_TYPE Find(symbol_table::SymbolTableItem &x);
    saERRORS::ERROR_TYPE Insert(const symbol_table::SymbolTableItem &x);
    void init();
    void BlockExit();
    void BlockIn(std::string name);
    void DoProgram(pascal2c::ast::Program x);
    symbol_table::MegaType MaxType(symbol_table::MegaType x,symbol_table::MegaType y);
    symbol_table::MegaType GetExprType(std::shared_ptr<pascal2c::ast::Expression> x);
    symbol_table::ItemType BasicToType(int basic_type);
    symbol_table::SymbolTableItem ExprToItem(std::string name, std::shared_ptr<pascal2c::ast::Expression> x);
    std::vector<symbol_table::SymbolTablePara> TypeToPara(pascal2c::ast::Type type);
    bool ParameterToPara(std::vector<symbol_table::SymbolTablePara> &ret,pascal2c::ast::Parameter inpara);
    symbol_table::SymbolTableItem SubprogramToItem(pascal2c::ast::SubprogramHead x);
    symbol_table::SymbolTableItem VarToItem(pascal2c::ast::Variable x);
    void DoProgramHead(pascal2c::ast::ProgramHead x);
    void DoProgramBody(pascal2c::ast::ProgramBody x);
    void DoConstDeclaration(pascal2c::ast::ConstDeclaration x);
    void DoVarDeclaration(pascal2c::ast::VarDeclaration x);
    void DoSubprogram(pascal2c::ast::Subprogram x);
    symbol_table::SymbolTableItem DoSubprogramHead(pascal2c::ast::SubprogramHead x);
    void DoSubprogramBody(pascal2c::ast::SubprogramBody x);
    void DoAllStatement(std::vector<std::shared_ptr<pascal2c::ast::Statement>> x);
    void DoAssignStatement(pascal2c::ast::AssignStatement x);
    void DoCallStatement(pascal2c::ast::CallStatement x);
    void DoCompoundStatement(pascal2c::ast::CompoundStatement x);
    void DoIfStatement(pascal2c::ast::IfStatement x);
    void DoForStatement(pascal2c::ast::ForStatement x);

    // I want to use the symbol table. Maybe this way is right?
    extern nameTable table;
}//end namespace analysiser
#endif