#include "semantic_analysis.h"

namespace analysiser{
    bool nameTable::Add(std::string name, std::shared_ptr<symbol_table::SymbolTableBlock> block)
    {
        table_[name]=block;
        if(table_.find(name)!=table_.end())
            return true;
        else 
            return false;
    }
    bool nameTable::Query(std::string name, std::shared_ptr<symbol_table::SymbolTableBlock> &ansblock)
    {
        if(table_.find(name)==table_.end())
            return false;
        else 
        {
            ansblock=table_[name];
            return true;
        }
    }
    void init()
    {
        nowblockName = "main";
    }
    void BlockExit()
    {
        nowblockName = blockNames[blockNames.size()-1];
        blockNames.pop_back();
    }
    void BlockIn(std::string name)
    {
        blockNames.push_back(nowblockName);
        nowblockName = name;
    }
    //TODO:
    //change class in ast to ItemType
#ifdef ASTFINISHED
    symbol_table::SymbolTableItem ProgramToItem(pascal2c::ast::ProgramHead program)
    {

    }
    symbol_table::SymbolTableItem ExprToItem(std::string name, pascal2c::ast::Expression expr)
    {

    }
    symbol_table::SymbolTableItem ExprToItem(pascal2c::ast::Expression expr)
    {

    }
    symbol_table::SymbolTableItem TypeToItem(std::string name, pascal2c::ast::Type type)
    {

    }
    vector<symbol_table::SymbolTablePara> ParameterToPara(pascal2c::ast::Parameter inpara)
    {

    }
    symbol_table::SymbolTableItem SubprogramToItem(pascal2c::ast::SubprogramHead subProgram)
    {

    }
#endif
    void DoProgram(/*Program A*/)
    {
        DoProgramHead(/*A.ProgramHead*/);
        DoProgramBody(/*A.ProgramBody*/);
    }
    void DoProgramHead(/*ProgramHead A*/)
    {
        //TODO: create SymbolTableItem itemA=ProgramToItem(programHead)
        //      BlockIn(programHead.id);
        //      add itemA to nowBlockName
    }
    void DoProgramBody(/*ProgramBody A*/)
    {
        DoConstDeclaration();
        DoVarDeclaration();
        DoSubprogram();
        DoAllStatement();
        BlockExit();
    }
    void DoConstDeclaration(/*constDeclaration*/)
    {
        //TODO: create SymbolTableItem itemA=ExprToItem(constDeclaration.id,constDeclaration.const_value)
        //      add itemA to nowBlockName
    }
    void DoVarDeclaration(/*varDeclaration*/)
    {
        //TODO: for i in varDeclaration.id_list:
        //          create SymbolTableItem itemA=TypeToItem(i.id,varDeclaration.Type)
        //          add itemA to nowBlockName
    }
    void DoSubprogram()
    {
        DoSubprogramHead();
        DoSubprogramBody();
    }
    void DoSubprogramHead()
    {
        //TODO:create SymbolTableItem itemA=SubprogramToItem(subprogramHead)
        //     BlockIn(subProgramHead.id)
        //     add itemA to nowBlockName
    }
    void DoSubprogramBody()
    {
        DoConstDeclaration();
        DoVarDeclaration();
        DoAllStatement();
        BlockExit();
    }
    void DoAllStatement()
    {
        //TODO:for i in statement_list
        //     hand out i DoXXX()
    }
    void DoAssignStatement()
    {
        //TODO:get var_,expr from ast
        //     find var_ in nowBlockName
        //     check type(Expr)==type(var_)
    }
    void DoCallStatement()
    {
        //TODO:get name from ast
        //     for i in expr_list, paralist.push_back(ExprToItem(i));
        //     make itemA with name and paralist,
        //     find itemA in nowBlockName
    }
    void DoCompoundStatement()
    {
        DoAllStatement();
    }
    void DoIfStatement()
    {
        //TODO:get condition from ast
        //     ExprToItem(condition) check type(condition)==boolean
        DoAllStatement();//then_
        DoAllStatement();//else_
    }
    void DoForStatement()
    {
        //TODO:get id,from,to from ast
        //     find id in nowBlockName
        //     check type(id)!=real,type(id)==type(from)==type(to)
        DoAllStatement();//statement_
    }
}//end namespace analysiser