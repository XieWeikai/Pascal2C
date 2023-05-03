#include <map>
#include <set>
#include <assert.h>
#include "../ast/ast.h"
#include"symbol_table.h"

using namespace symbol_table;



//add identify with format SymbolTableItem
//return 0 if success; otherwise failure
saERRORS::ERROR_TYPE SymbolTableBlock::AddItem(const SymbolTableItem &x)
{
	std::string name=x.name();
	if (this->name_domain.find(name)==this->name_domain.end())
	{
		this->name_domain[name]=x.is_func();
		this->table[name].insert(x);
		return saERRORS::NO_ERROR;
	}
	bool A=this->name_domain[name],B=x.is_func();
	if (A!=B) return saERRORS::ITEM_ERROR;//it is a var not func
	if (!A) return saERRORS::ITEM_EXIST;//var redefinition
	if (this->table[name].find(x)!=this->table[name].end()) return saERRORS::ITEM_EXIST;//func redefinition
	this->table[name].insert(x);
	return saERRORS::NO_ERROR;
}

bool isadapt(const std::vector<SymbolTablePara> &A,const std::vector<SymbolTablePara> &B)
{
	assert(A.size()==B.size());
	for (int i=0;i<A.size();i++) if (!A[i].is_var() && B[i].is_var()) return false;
	return true;
}
//find identify with format SymbolTableItem
//do not care about para.info_
//return if exist
saERRORS::ERROR_TYPE SymbolTableBlock::Query(SymbolTableItem &x)
{
	std::string name=x.name();
	for (std::shared_ptr<SymbolTableBlock> nw=std::make_shared<SymbolTableBlock>(*this);nw;nw=nw->father)
	{
		if (nw->name_domain.find(name)!=nw->name_domain.end())
		{
			bool A=nw->name_domain[name],B=x.is_func();
			if (A!=B) return saERRORS::FOUND_BUT_NOT_MATCH;//func and variable dismatch
			if (!A)//variable
			{
				auto temp=nw->table[name].begin();
				int P=temp->para().size(),Q=x.para().size();
				if (P<Q) return saERRORS::FOUND_BUT_NOT_MATCH;//variable:too long parameter
				for (auto &o:x.para()) if (o.type()!=ItemType::INT) return saERRORS::FOUND_BUT_NOT_MATCH;//expect int but other
				x=SymbolTableItem(temp->type(),temp->name(),temp->is_var(),temp->is_func(),x.para());
				return saERRORS::NO_ERROR;
			}
			auto temp=nw->table[name].find(x);
			if (temp!=nw->table[name].end())
			{
				if (!isadapt(x.para(),temp->para())) return saERRORS::FOUND_BUT_NOT_MATCH;
				x=*temp;
				return saERRORS::NO_ERROR;
			}
			else return saERRORS::FOUND_BUT_NOT_MATCH;
		}
	}
	if (x.name()=="read" || x.name()=="readln")
	{
		for (auto &temp:x.para()) if (!temp.is_var() || temp.type()==ERROR || temp.type()==VOID) return saERRORS::FOUND_BUT_NOT_MATCH;//found name but parameter dismatch
		x.settype(ItemType::VOID);
		return saERRORS::NO_ERROR;
	}
	if (x.name()=="write" || x.name()=="writeln")
	{
		for (auto &temp:x.para()) if (temp.type()==ERROR || temp.type()==VOID) return saERRORS::FOUND_BUT_NOT_MATCH;//found name but parameter dismatch
		x.settype(ItemType::VOID);
		return saERRORS::NO_ERROR;
	}
	return saERRORS::NOT_FOUND;
}
std::shared_ptr<SymbolTableBlock> SymbolTableBlock::getfather(){return father;}