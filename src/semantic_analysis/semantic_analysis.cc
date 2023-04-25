#include <memory>
#include "semantic_analysis.h"
#include "../ast/program.h"

#define LOG(message) do{printf("ERROR in line(%d),column(%d):%s\n",x.line(),x.column(),message);}while(0)
namespace analysiser{
    bool nameTable::Add(std::string name)
    {
        std::shared_ptr<symbol_table::SymbolTableBlock> block;
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
    bool Find(symbol_table::SymbolTableItem &x)
    {
        std::shared_ptr<symbol_table::SymbolTableBlock> ans;
        table.Query(nowblockName,ans);
        return ans->Query(x);
    }
    bool Insert(symbol_table::SymbolTableItem &x)
    {
        if(x.type()==symbol_table::ERROR)
        {
            return false;
        }
        std::shared_ptr<symbol_table::SymbolTableBlock> nowBlock;
        table.Query(nowblockName,nowBlock);
        nowBlock->AddItem(x);
        return true;
    }
    void init()
    {
        nowblockName = "__main__";
        table.Add(nowblockName);
    }
    void BlockExit()
    {
        nowblockName = blockNames[blockNames.size()-1];
        blockNames.pop_back();
    }
    void BlockIn(std::string name)
    {
        blockNames.push_back(nowblockName);
        table.Add(name);
        nowblockName = name;
    }
    //TODO:
    //change class in ast to ItemType
    symbol_table::ItemType MaxType(symbol_table::ItemType x, symbol_table::ItemType y)
    {
        if(x==symbol_table::ERROR||y==symbol_table::ERROR)
        {
            return symbol_table::ERROR;
        }
        if((x!=symbol_table::INT && x!=symbol_table::REAL)||(y!=symbol_table::INT && y!=symbol_table::REAL))
        {
            if(x!=y)
                return symbol_table::ERROR;
            else 
                return x;
        }
        else 
        {
            if(x==symbol_table::REAL||y==symbol_table::REAL)
                return symbol_table::REAL;
            else 
                return symbol_table::INT;
        }
    }
    bool ExprIsVar(std::shared_ptr<pascal2c::ast::Expression> x)
    {
        switch(x->GetType())
        {
            case pascal2c::ast::INT:
            case pascal2c::ast::REAL:
            case pascal2c::ast::CHAR:
            case pascal2c::ast::BOOLEAN:
                return false;
            case pascal2c::ast::CALL_OR_VAR:
            case pascal2c::ast::VARIABLE:
            case pascal2c::ast::CALL:
                return true;
            case pascal2c::ast::BINARY:
            {
                std::shared_ptr<pascal2c::ast::BinaryExpr> now=std::static_pointer_cast<pascal2c::ast::BinaryExpr>(x);
                return ExprIsVar(now->lhs())&&ExprIsVar(now->rhs());
            }
            case pascal2c::ast::UNARY:
            {
                return ExprIsVar(std::static_pointer_cast<pascal2c::ast::UnaryExpr>(x)->factor());
            }
        }
    }
    symbol_table::ItemType GetExprType(std::shared_ptr<pascal2c::ast::Expression> x)
    {
        symbol_table::ItemType ret=symbol_table::ERROR;
        switch(x->GetType())
        {
            case pascal2c::ast::INT:
            {
                ret=symbol_table::INT;
                break;
            }
            case pascal2c::ast::REAL:
            {
                ret=symbol_table::REAL;
                break;
            }
            case pascal2c::ast::CHAR: 
            {
                ret=symbol_table::CHAR;
                break;
            }
            case pascal2c::ast::BOOLEAN:
            {
                ret=symbol_table::BOOL;
                break;
            }
            case pascal2c::ast::VARIABLE:
            {
                std::shared_ptr<pascal2c::ast::Variable> now=std::static_pointer_cast<pascal2c::ast::Variable>(x);
                std::vector<symbol_table::SymbolTablePara> para;
                for(auto i:now->expr_list())
                {
                    para.push_back(symbol_table::SymbolTablePara(GetExprType(i),true,""));
                }
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),true,false,para);
                if(Find(tgt1))
                {
                    ret=tgt1.type();
                }
                break;
            }
            case pascal2c::ast::CALL:
            {
                std::shared_ptr<pascal2c::ast::CallValue> now=std::static_pointer_cast<pascal2c::ast::CallValue>(x);
                std::vector<symbol_table::SymbolTablePara> para;
                for(auto i:now->params())
                {
                    para.push_back(symbol_table::SymbolTablePara(GetExprType(i),ExprIsVar(i),""));
                }
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),false,true,para);
                if(Find(tgt1))
                {
                    ret=tgt1.type();
                }
                break;
            }
            case pascal2c::ast::CALL_OR_VAR:
            {
                std::shared_ptr<pascal2c::ast::CallOrVar> now=std::static_pointer_cast<pascal2c::ast::CallOrVar>(x);
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),true,false,std::vector<symbol_table::SymbolTablePara>());
                symbol_table::SymbolTableItem tgt2(symbol_table::ERROR,now->id(),false,true,std::vector<symbol_table::SymbolTablePara>());
                if(Find(tgt1))
                {
                    ret=tgt1.type();
                }
                else if(Find(tgt2))
                {
                    ret=tgt2.type();
                }
                break;
            }
            case pascal2c::ast::BINARY:
                ret=MaxType(GetExprType(std::static_pointer_cast<pascal2c::ast::BinaryExpr>(x)->lhs()),
                            GetExprType(std::static_pointer_cast<pascal2c::ast::BinaryExpr>(x)->rhs()));break;
            case pascal2c::ast::UNARY: 
                ret=GetExprType(std::static_pointer_cast<pascal2c::ast::UnaryExpr>(x)->factor());break;
        }
        return ret;
    }
    symbol_table::ItemType BasicToType(int basic_type)
    {
        //Todo 
        return symbol_table::ERROR;
    }
    symbol_table::SymbolTableItem ExprToItem(std::string name, std::shared_ptr<pascal2c::ast::Expression> x)
    {
        symbol_table::ItemType itemtype=GetExprType(x);
        symbol_table::SymbolTableItem ret(itemtype,name,false,false,std::vector<symbol_table::SymbolTablePara>());
        return ret;
    }
    std::vector<symbol_table::SymbolTablePara> TypeToPara(pascal2c::ast::Type type)
    {
        std::vector<symbol_table::SymbolTablePara> ret;
        if(type.is_array())
        {
            for(auto i : type.periods())
            {
                ret.push_back(symbol_table::SymbolTablePara(BasicToType(type.basic_type()),false,std::to_string(i.digits_1)+"..."+std::to_string(i.digits_2)));
            }
        }
        return ret;
    }
    bool ParameterToPara(std::vector<symbol_table::SymbolTablePara> ret,pascal2c::ast::Parameter inpara)
    {
        for(int i=0;i<inpara.id_list()->Size();i++)
        {
            Insert(symbol_table::SymbolTableItem(BasicToType(inpara.type()),(*inpara.id_list())[i],inpara.is_var(),false,std::vector<symbol_table::SymbolTablePara>()));
            ret.push_back(symbol_table::SymbolTablePara(BasicToType(inpara.type()),inpara.is_var(),""));
        }
        return true;
    }
    symbol_table::SymbolTableItem SubprogramToItem(pascal2c::ast::SubprogramHead x)
    {
        std::vector<symbol_table::SymbolTablePara> para;
        symbol_table::ItemType itemtype=BasicToType(x.return_type());
        for(auto i : x.parameters())
        {
            if(!ParameterToPara(para,*i))
            {
                itemtype=symbol_table::ERROR;
            }
        }
        return symbol_table::SymbolTableItem(itemtype,x.id(),false,true,para);
    }
    symbol_table::SymbolTableItem VarToItem(pascal2c::ast::Variable x)
    {
        std::vector<symbol_table::SymbolTablePara> para;
        for(auto i:x.expr_list())
        {
            para.push_back(symbol_table::SymbolTablePara(GetExprType(i),ExprIsVar(i),""));
        }
        return symbol_table::SymbolTableItem(symbol_table::ERROR,x.id(),true,false,para);
    }

    void DoProgram(pascal2c::ast::Program x)
    {
        DoProgramHead(*x.program_head());
        DoProgramBody(*x.program_body());
    }
    void DoProgramHead(pascal2c::ast::ProgramHead x)
    {
        BlockIn(x.id());
    }
    void DoProgramBody(pascal2c::ast::ProgramBody x)
    {
        for(auto i:x.const_declarations())
        {
            DoConstDeclaration(*i);
        }
        for(auto i:x.var_declarations())
        {
            DoVarDeclaration(*i);
        }
        for(auto i:x.subprogram_declarations())
        {
            DoSubprogram(*i);
        }
        DoAllStatement(x.statement_list());
        BlockExit();
    }
    void DoConstDeclaration(pascal2c::ast::ConstDeclaration x)
    {
        symbol_table::SymbolTableItem itemA = ExprToItem(x.id(),x.const_value());
        if(!Insert(itemA))
        {
            LOG("Const Declaration failure");
        }
    }
    void DoVarDeclaration(pascal2c::ast::VarDeclaration x)
    {
        std::vector<symbol_table::SymbolTablePara> para = TypeToPara(*x.type());
        for(int i=0;i<x.id_list()->Size();i++)
        {
            symbol_table::SymbolTableItem now(BasicToType(x.type()->basic_type()),(*x.id_list())[i],false,false,para);
            if(!Insert(now))
            {
                LOG("Var Declaration failure");
            }
        }
    }
    void DoSubprogram(pascal2c::ast::Subprogram x)
    {
        symbol_table::SymbolTableItem now=DoSubprogramHead(*x.subprogram_head());
        DoSubprogramBody(*x.subprogram_body());
        if(!Insert(now))
        {
            LOG("Subprogram Declaration failure");
        }
    }
    symbol_table::SymbolTableItem DoSubprogramHead(pascal2c::ast::SubprogramHead x)
    {
        BlockIn(x.id());
        symbol_table::SymbolTableItem now=SubprogramToItem(x);
        if(!Insert(now))
        {
            LOG("SubprogramHead Declaration failure");
        }
        return now;
    }
    void DoSubprogramBody(pascal2c::ast::SubprogramBody x)
    {
        for(auto i:x.const_declarations())
        {
            DoConstDeclaration(*i);
        }
        for(auto i:x.var_declarations())
        {
            DoVarDeclaration(*i);
        }
        DoAllStatement(x.statement_list());
        BlockExit();
    }
    void DoAllStatement(std::vector<std::shared_ptr<pascal2c::ast::Statement>> x)
    {
        for(auto i:x)
        {
            switch(i->GetType())
            {
                case pascal2c::ast::ASSIGN_STATEMENT: 
                    DoAssignStatement(*(std::static_pointer_cast<pascal2c::ast::AssignStatement>(i)));break;
                case pascal2c::ast::CALL_STATEMENT: 
                    DoCallStatement(*(std::static_pointer_cast<pascal2c::ast::CallStatement>(i)));break;
                case pascal2c::ast::COMPOUND_STATEMENT: 
                    DoCompoundStatement(*(std::static_pointer_cast<pascal2c::ast::CompoundStatement>(i)));break;
                case pascal2c::ast::IF_STATEMENT:
                    DoIfStatement(*(std::static_pointer_cast<pascal2c::ast::IfStatement>(i)));break;
                case pascal2c::ast::FOR_STATEMENT: 
                    DoForStatement(*(std::static_pointer_cast<pascal2c::ast::ForStatement>(i)));break;
            }
        }
    }
    void DoAssignStatement(pascal2c::ast::AssignStatement x)
    {
        symbol_table::SymbolTableItem l=VarToItem(*x.var());
        symbol_table::ItemType ltype;
        if(Find(l))
        {
            ltype=l.type();
        }
        else 
        {
            LOG("Assign Statement failure(left not found)");
            return;
        }
        symbol_table::ItemType rtype=GetExprType(x.expr());
        if(rtype==symbol_table::ERROR)
        {
            LOG("Assign Statement failure(right not found)");
            return;
        }
        if(MaxType(ltype,rtype)!=ltype)
        {
            LOG("Assign Statement failure(type not match)");
            return;
        }
    }
    void DoCallStatement(pascal2c::ast::CallStatement x)
    {
        std::vector<symbol_table::SymbolTablePara> para;
        for(auto i:x.expr_list())
        {
            para.push_back(symbol_table::SymbolTablePara(GetExprType(i),ExprIsVar(i),""));
        }
        symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,x.name(),false,true,para);
        if(!Find(tgt1))
        {
            LOG("Call Statement failure(function not found)");
            return;
        }
    }
    void DoCompoundStatement(pascal2c::ast::CompoundStatement x)
    {
        DoAllStatement(x.statements());
    }
    void DoIfStatement(pascal2c::ast::IfStatement x)
    {
        if(GetExprType(x.condition())!=symbol_table::BOOL)
        {
            LOG("If Statement failure(condition error)");
            return;
        }
        //TODO:get condition from ast
        //     ExprToItem(condition) check type(condition)==boolean
        std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
        inp.push_back(x.then());
        DoAllStatement(inp);//then_
        inp.pop_back();
        inp.push_back(x.else_part());
        DoAllStatement(inp);//else_
    }
    void DoForStatement(pascal2c::ast::ForStatement x)
    {
        symbol_table::SymbolTableItem tgt(symbol_table::ERROR,x.id(),true,false,std::vector<symbol_table::SymbolTablePara>());
        if(!Find(tgt))
        {
            LOG("For Statement failure(id not found)");
        }
        symbol_table::ItemType fromtype=GetExprType(x.from());
        symbol_table::ItemType totype=GetExprType(x.to());
        if(MaxType(MaxType(fromtype,totype),tgt.type())==symbol_table::ERROR)//TODO
        {
            LOG("For Statement failure(type not match in id from to)");
            return;
        }
        std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
        inp.push_back(x.statement());
        DoAllStatement(inp);
    }
}//end namespace analysiser