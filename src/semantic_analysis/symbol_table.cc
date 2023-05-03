#include <map>
#include <set>
#include <assert.h>
#include "../ast/ast.h"
#include"symbol_table.h"

using namespace symbol_table;



//add identify with format SymbolTableItem
//return 0 if success; otherwise failure
int SymbolTableBlock::AddItem(const SymbolTableItem &x)
{
	std::string name=x.name();
	if (this->table[name].find(x)!=this->table[name].end()) return -1;
	this->table[name].insert(x);return 0;
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
		if (nw->table.find(name)!=nw->table.end())
		{
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
		if (!x.para().size()) return saERRORS::FOUND_BUT_NOT_MATCH;
		for (auto &temp:x.para()) if (!temp.is_var() || temp.type()==ERROR || temp.type()==VOID) return saERRORS::FOUND_BUT_NOT_MATCH;//found name but parameter dismatch
		return saERRORS::NO_ERROR;
	}
	if (x.name()=="write" || x.name()=="writeln")
	{
		if (!x.para().size()) return saERRORS::FOUND_BUT_NOT_MATCH;
		for (auto &temp:x.para()) if (temp.type()==ERROR || temp.type()==VOID) return saERRORS::FOUND_BUT_NOT_MATCH;//found name but parameter dismatch
		return saERRORS::NO_ERROR;
	}
	return saERRORS::NOT_FOUND;
}
std::shared_ptr<SymbolTableBlock> SymbolTableBlock::getfather(){return father;}