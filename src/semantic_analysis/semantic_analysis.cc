#include <memory>
#include <sstream>
#include "semantic_analysis.h"
#define LOG(message) \
    do{\
        errors.push_back(analysiser::errorMsg(x.line(),x.column(),message));\
    }while(0)
namespace analysiser{
    std::vector<std::string> blockNames;//memary name of the latest block
    std::string nowblockName; 
    nameTable table;
    std::vector<errorMsg> errors;
    std::vector<errorMsg> GetErrors()    {return errors;}
    nameTable* GetTable() {return &table;}

    bool nameTable::Add(std::string name)
    {
        table_[name]=std::make_shared<symbol_table::SymbolTableBlock>(symbol_table::SymbolTableBlock());
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
    saERRORS::ERROR_TYPE Find(symbol_table::SymbolTableItem &x)
    {
        std::shared_ptr<symbol_table::SymbolTableBlock> ans;
        table.Query(nowblockName,ans);
        return ans->Query(x);
    }
    saERRORS::ERROR_TYPE Insert(const symbol_table::SymbolTableItem &x)
    {
        if(x.type()==symbol_table::ERROR)
        {
            return saERRORS::ITEM_ERROR;
        }
        std::shared_ptr<symbol_table::SymbolTableBlock> nowBlock;
        table.Query(nowblockName,nowBlock);
        return nowBlock->AddItem(x);
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
        std::shared_ptr<symbol_table::SymbolTableBlock> af;
        std::shared_ptr<symbol_table::SymbolTableBlock> as;
        table.Query(nowblockName,af);
        table.Query(name,as);
        as->Locate(af);
        nowblockName = name;
    }
    symbol_table::MegaType MaxType(symbol_table::MegaType x, symbol_table::MegaType y)
    {
        if(x.type()==symbol_table::ERROR||y.type()==symbol_table::ERROR||x.pointer().size()!=y.pointer().size())
        {
            return symbol_table::MegaType(symbol_table::ERROR);
        }
        if(x.pointer().size()!=0||(x.type()!=symbol_table::INT && x.type()!=symbol_table::REAL)||(y.type()!=symbol_table::INT && y.type()!=symbol_table::REAL))
        {
            if(x!=y)
                return symbol_table::MegaType(symbol_table::ERROR);
            else 
                return x;
        }
        else 
        {
            if(x.type()==symbol_table::REAL||y.type()==symbol_table::REAL)
                return symbol_table::MegaType(symbol_table::REAL);
            else 
                return symbol_table::MegaType(symbol_table::INT);
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
            case pascal2c::ast::STRING:
            case pascal2c::ast::CALL:
                return false;
            case pascal2c::ast::VARIABLE:
                return true;
            case pascal2c::ast::CALL_OR_VAR:
            {
                std::shared_ptr<pascal2c::ast::CallOrVar> now=std::static_pointer_cast<pascal2c::ast::CallOrVar>(x);
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),true,false,std::vector<symbol_table::SymbolTablePara>());
                if(Find(tgt1)==saERRORS::NO_ERROR)
                {
                    return true;
                }
                return false;
            }
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
        return false;
    }
    symbol_table::MegaType GetExprType(std::shared_ptr<pascal2c::ast::Expression> x)
    {
        symbol_table::MegaType ret(symbol_table::ERROR);
        switch(x->GetType())
        {
            case pascal2c::ast::INT:
            {
                ret.settype(symbol_table::INT);
                break;
            }
            case pascal2c::ast::REAL:
            {
                ret.settype(symbol_table::REAL);
                break;
            }
            case pascal2c::ast::CHAR: 
            {
                ret.settype(symbol_table::CHAR);
                break;
            }
            case pascal2c::ast::BOOLEAN:
            {
                ret.settype(symbol_table::BOOL);
                break;
            }
            case pascal2c::ast::STRING:
            {
                ret.settype(symbol_table::STRING);
                break;
            }
            case pascal2c::ast::VARIABLE:
            {
                std::shared_ptr<pascal2c::ast::Variable> now=std::static_pointer_cast<pascal2c::ast::Variable>(x);
                std::vector<symbol_table::SymbolTablePara> para;
                for(auto i:now->expr_list())
                {
                    para.push_back(symbol_table::SymbolTablePara(GetExprType(i),ExprIsVar(i),""));
                }
                symbol_table::SymbolTableItem tgt1=VarToItem(*now);
                if(Find(tgt1)==saERRORS::NO_ERROR)
                {
                    ret=tgt1.type();
                }
                else 
                {
                    pascal2c::ast::Ast x=(*now);
                    std::stringstream ss;
                    ss<<tgt1;
                    LOG("variable "+ss.str()+" not found");
                }
                break;
            }
            case pascal2c::ast::CALL:
            {
                std::shared_ptr<pascal2c::ast::CallValue> now=std::static_pointer_cast<pascal2c::ast::CallValue>(x);
                std::vector<symbol_table::SymbolTablePara> para;
                for(auto i:now->params())
                {
                    para.push_back(symbol_table::SymbolTablePara(GetExprType(i).type(),ExprIsVar(i),""));
                }
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),false,true,para);
                if(Find(tgt1)==saERRORS::NO_ERROR)
                {
                    ret=tgt1.type();
                }
                else 
                {
                    pascal2c::ast::Ast x=(*now);
                    LOG("function "+now->id()+" not found");
                }
                break;
            }
            case pascal2c::ast::CALL_OR_VAR:
            {
                std::shared_ptr<pascal2c::ast::CallOrVar> now=std::static_pointer_cast<pascal2c::ast::CallOrVar>(x);
                symbol_table::SymbolTableItem tgt1(symbol_table::ERROR,now->id(),true,false,std::vector<symbol_table::SymbolTablePara>());
                symbol_table::SymbolTableItem tgt2(symbol_table::ERROR,now->id(),false,true,std::vector<symbol_table::SymbolTablePara>());
                if(Find(tgt1)==saERRORS::NO_ERROR)
                {
                    ret=tgt1.type();
                }
                else if(Find(tgt2)==saERRORS::NO_ERROR)
                {
                    ret=tgt2.type();
                }
                else 
                {
                    pascal2c::ast::Ast x=(*now);
                    LOG(now->id()+" not found");
                }
                break;
            }
            case pascal2c::ast::BINARY:
            {
                std::shared_ptr<pascal2c::ast::BinaryExpr> now=std::static_pointer_cast<pascal2c::ast::BinaryExpr>(x);
                switch(now->op())
                {
                    case '=':case '>':case '<':case 306:case 305:case 304:
                    {
                        symbol_table::MegaType tyl=GetExprType(now->lhs());
                        symbol_table::MegaType tyr=GetExprType(now->rhs());
                        symbol_table::MegaType ty=MaxType(tyl,tyr);
                        if(ty!=symbol_table::ERROR)
                        {
                            ret.settype(symbol_table::BOOL);
                        }
                        else 
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::string mes="";
                            if(tyl!=symbol_table::ERROR&&tyr!=symbol_table::ERROR)
                            {
                                std::stringstream ss;
                                ss<<":left:"<<tyl<<" "<<"right:"<<tyr;
                                mes=ss.str();
                            }
                            LOG("illegal type between comparison expression"+mes);
                        }
                        break;
                    }
                    case 262:case 283:
                    {
                        symbol_table::MegaType ty=GetExprType(now->lhs());
                        if(ty!=symbol_table::BOOL)
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::stringstream ss;
                            std::string mes="";
                            ss<<ty;
                            mes=ss.str();
                            LOG("illegal type between boolean expression,got "+mes);
                            break;
                        }
                        ty=GetExprType(now->rhs());
                        if(ty!=symbol_table::BOOL)
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::stringstream ss;
                            std::string mes="";
                            ss<<ty;
                            mes=ss.str();
                            LOG("illegal type between boolean expression,got "+mes);
                            break;
                        }
                        ret.settype(symbol_table::BOOL);
                        break;
                    }
                    case '+':case '-':case '*':
                    {
                        symbol_table::MegaType ty=MaxType(GetExprType(now->lhs()),GetExprType(now->rhs()));
                        if(ty.pointer().size()!=0)
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::string mes="";
                            if(ty.type()!=symbol_table::ERROR)
                            {
                                std::stringstream ss;
                                ss<<":got ";
                                ss<<ty;
                                mes=ss.str(); 
                            }
                            LOG("illegal type between compute expression"+mes);
                        }
                        else if(ty.type()==symbol_table::REAL||ty.type()==symbol_table::INT)
                        {
                            ret.settype(ty.type());
                        }
                        else 
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::string mes=":type not match";
                            if(ty.type()!=symbol_table::ERROR)
                            {
                                std::stringstream ss;
                                ss<<":got "<<ty.type();
                                mes=ss.str();
                            }
                            LOG("illegal type between compute expression"+mes);
                        }
                        break;
                    }
                    case 267:
                    {
                        symbol_table::MegaType lty=GetExprType(now->lhs()),rty=GetExprType(now->rhs());
                        if(lty.type()==symbol_table::INT&&rty.type()==symbol_table::INT)
                        {
                            ret.settype(symbol_table::INT);
                        }
                        else 
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::stringstream ss;
                            ss<<":got ";
                            ss<<lty<<" "<<rty;
                            LOG("illegal type between compute expression"+ss.str());
                        }
                        break;
                    }
                    case 279:
                    {
                        if(GetExprType(now->lhs())==symbol_table::INT&&GetExprType(now->rhs())==symbol_table::INT)
                        {
                            ret.settype(symbol_table::INT);
                        }
                        else 
                        {
                            pascal2c::ast::Ast x=(*now);
                            LOG("illegal type in mod expression");
                        }
                        break;
                    }
                    case '/':
                    {
                        symbol_table::MegaType ty=MaxType(GetExprType(now->lhs()),GetExprType(now->rhs()));
                        if(ty.pointer().size()!=0)
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::string mes="";
                            if(ty.type()!=symbol_table::ERROR)
                            {
                                std::stringstream ss;
                                ss<<":got ";
                                ss<<ty;
                                mes=ss.str(); 
                            }
                            LOG("illegal type between compute expression"+mes);
                        }
                        else if(ty.type()==symbol_table::REAL||ty.type()==symbol_table::INT)
                        {
                            ret.settype(symbol_table::REAL);
                        }
                        else 
                        {
                            pascal2c::ast::Ast x=(*now);
                            std::stringstream ss;ss<<GetExprType(now->lhs());
                            LOG("illegal type in '/' expression"+ss.str());
                        }
                        break;
                    }
                }
                break;
            }
            case pascal2c::ast::UNARY: 
            {
                std::shared_ptr<pascal2c::ast::UnaryExpr> now=std::static_pointer_cast<pascal2c::ast::UnaryExpr>(x);
                symbol_table::MegaType ty=GetExprType(now->factor());
                if(ty.pointer().size()!=0)
                {
                    pascal2c::ast::Ast x=(*now);
                    std::string mes="";
                    if(ty.type()!=symbol_table::ERROR)
                    {
                        std::stringstream ss;
                        ss<<":got ";
                        ss<<ty;
                        mes=ss.str(); 
                    }
                    LOG("illegal type in unary expression"+mes);
                }
                else if(now->op()==281)
                {
                    if(ty.type()!=symbol_table::BOOL)
                    {
                        pascal2c::ast::Ast x=(*now);
                        std::stringstream ss;
                        ss<<ty.type();
                        LOG("illegal type in unary expression:got "+ss.str());
                    }
                    else
                    {
                        ret.settype(symbol_table::BOOL);
                    }
                    break;
                }
                if(ty.type()==symbol_table::REAL||ty.type()==symbol_table::INT)
                {
                    ret.settype(ty.type());
                }
                else 
                {
                    pascal2c::ast::Ast x=(*now);
                    LOG("illegal type in unary expression");
                }
                break;
            }
        }
        return ret;
    }
    symbol_table::ItemType BasicToType(int basic_type)
    {
        if(basic_type==297)
            return symbol_table::INT;
        else if(basic_type==298)
            return symbol_table::REAL;
        else if(basic_type==299)
            return symbol_table::BOOL;
        else if(basic_type==300)
            return symbol_table::CHAR;
        else if(basic_type==-1)
            return symbol_table::VOID;
        return symbol_table::ERROR;
    }
    symbol_table::SymbolTableItem ExprToItem(std::string name, std::shared_ptr<pascal2c::ast::Expression> x)
    {
        symbol_table::MegaType itemtype=GetExprType(x);
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
                ret.push_back(symbol_table::SymbolTablePara(symbol_table::INT,false,std::to_string(i.digits_1)+".."+std::to_string(i.digits_2)));
            }
        }
        return ret;
    }
    bool ParameterToPara(
        std::vector<symbol_table::SymbolTablePara> &ret,
        pascal2c::ast::Parameter inpara)
    {
        for(int i=0;i<inpara.id_list()->Size();i++)
        {
            Insert(symbol_table::SymbolTableItem(
                BasicToType(inpara.type()) ,
                    (*inpara.id_list())[i] ,
                    true ,false ,
                    std::vector<symbol_table::SymbolTablePara>{}));
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
        BlockIn(std::to_string(x.line()));
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
        if(x.statements())
        {
            std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
            inp.push_back(x.statements());
            DoAllStatement(inp);
        }
        BlockExit();
    }
    void DoConstDeclaration(pascal2c::ast::ConstDeclaration x)
    {
        symbol_table::SymbolTableItem itemA = ExprToItem(x.id(),x.const_value());
        saERRORS::ERROR_TYPE err=Insert(itemA);
        if(err!=saERRORS::NO_ERROR)
        {
            LOG("Const Declaration failure("+saERRORS::toString(err)+")");
        }
    }
    void DoVarDeclaration(pascal2c::ast::VarDeclaration x)
    {
        std::vector<symbol_table::SymbolTablePara> para = TypeToPara(*x.type());
        for(int i=0;i<x.id_list()->Size();i++)
        {
            symbol_table::SymbolTableItem now(BasicToType(x.type()->basic_type()),(*x.id_list())[i],true,false,para);
            saERRORS::ERROR_TYPE err=Insert(now);
            if(err!=saERRORS::NO_ERROR)
            {
                LOG("Var Declaration failure("+saERRORS::toString(err)+")");
            }
        }
    }
    void DoSubprogram(pascal2c::ast::Subprogram x)
    {
        std::string nameTmp=nowblockName;
        symbol_table::SymbolTableItem now=DoSubprogramHead(*x.subprogram_head());
        //insert subprogram head
        if(now.type()==symbol_table::ERROR)
        {
            LOG("Subprogram Declaration failure");
        }
        std::shared_ptr<symbol_table::SymbolTableBlock> tmpBlock;
        table.Query(nameTmp,tmpBlock);
        if(tmpBlock->AddItem(now)!=saERRORS::NO_ERROR)
        {
            std::stringstream ss;
            ss<<now;
            LOG("Subprogram Declaration failure(insert "+ss.str()+" error)");
        }

        DoSubprogramBody(*x.subprogram_body());
    }
    symbol_table::SymbolTableItem DoSubprogramHead(pascal2c::ast::SubprogramHead x)
    {
        BlockIn(std::to_string(x.line()));
        symbol_table::SymbolTableItem now=SubprogramToItem(x);
        if(now.type()!=symbol_table::VOID)
        {
            symbol_table::SymbolTableItem nownext(now.type(),x.id(),true,true,std::vector<symbol_table::SymbolTablePara>());
            saERRORS::ERROR_TYPE err=Insert(nownext);
            if(err!=saERRORS::NO_ERROR)
            {
                std::stringstream ss;
                ss<<nownext;
                LOG("SubprogramHead Declaration failure(insert "+ss.str()+" error)");
            }
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
        if(x.statement_list())
        {
            std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
            inp.push_back(x.statement_list());
            DoAllStatement(inp);
        }
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
                case pascal2c::ast::WHILE_STATEMENT:
                    DoWhileStatement(*(std::static_pointer_cast<pascal2c::ast::WhileStatement>(i)));break;
            }
        }
    }
    void DoAssignStatement(pascal2c::ast::AssignStatement x)
    {
        symbol_table::SymbolTableItem l=VarToItem(*x.var());
        symbol_table::MegaType ltype;
        if(Find(l)==saERRORS::NO_ERROR)
        {
            ltype=l.type();
            if(l.is_var()==false)
            {
                LOG("Assign Statement failure(left is const)");
                return;
            }
        }
        else 
        {
            LOG("Assign Statement failure(left not found)");
            return;
        }
        symbol_table::MegaType rtype=GetExprType(x.expr());
        if(rtype.type()==symbol_table::ERROR)
        {
            LOG("Assign Statement failure(right not found)");
            return;
        }
        if(MaxType(ltype,rtype)!=ltype)
        {
            std::stringstream ss;
            ss<<": left:"<<ltype<<" right:"<<rtype;
            LOG("Assign Statement failure(type not match)"+ss.str());
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
        saERRORS::ERROR_TYPE err = Find(tgt1);
        if(err!=saERRORS::NO_ERROR)
        {
            std::stringstream ss;
            ss<<tgt1;
            LOG("Call Statement failure("+saERRORS::toString(err)+",asking for "+ss.str()+")");
            return;
        }
    }
    void DoCompoundStatement(pascal2c::ast::CompoundStatement x)
    {
        DoAllStatement(x.statements());
    }
    void DoIfStatement(pascal2c::ast::IfStatement x)
    {
        symbol_table::MegaType ty=GetExprType(x.condition());
        if(ty!=symbol_table::BOOL)
        {
            std::stringstream ss;
            ss<<ty;
            LOG("If Statement failure(condition error:received type "+ss.str()+")");
            return;
        }
        std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
        if(x.then())
        {
            inp.push_back(x.then());
            DoAllStatement(inp);//then_
            inp.pop_back();
        }
        if(x.else_part())
        {
            inp.push_back(x.else_part());
            DoAllStatement(inp);//else_
        }
    }
    void DoForStatement(pascal2c::ast::ForStatement x)
    {
        symbol_table::SymbolTableItem tgt(symbol_table::ERROR,x.id(),true,false,std::vector<symbol_table::SymbolTablePara>());
        if(Find(tgt)!=saERRORS::NO_ERROR)
        {
            LOG("For Statement failure(id: "+x.id()+" not found)");
            return;
        }
        symbol_table::MegaType fromtype=GetExprType(x.from());
        symbol_table::MegaType totype=GetExprType(x.to());
        if(fromtype!=totype||totype!=tgt.type()||tgt.type()!=symbol_table::INT)//TODO
        {
            std::stringstream ss;
            ss<<":id:"<<tgt.type()<<" from:"<<fromtype<<" to:"<<totype;
            LOG("For Statement failure(type error in id from to)"+ss.str());
            return;
        }
        if(x.statement())
        {
            std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
            inp.push_back(x.statement());
            DoAllStatement(inp);
        }
    }
    void DoWhileStatement(pascal2c::ast::WhileStatement x)
    {
        symbol_table::MegaType ty=GetExprType(x.condition());
        if(ty!=symbol_table::BOOL)
        {
            std::stringstream ss;
            ss<<ty;
            LOG("While Statement failure(condition error:received type "+ss.str()+")");
            return;
        }
        if(x.statement())
        {
            std::vector<std::shared_ptr<pascal2c::ast::Statement>> inp;
            inp.push_back(x.statement());
            DoAllStatement(inp);
        }
    }
}//end namespace analysiser