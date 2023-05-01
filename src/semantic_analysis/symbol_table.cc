#include <map>
#include <set>
#include <assert.h>
#include "../ast/ast.h"
#include"symbol_table.h"

using namespace symbol_table;



istream& operator>>(istream& IN,ItemType& x)
{
	std::string s;IN>>s;
	if (s=="VOID") x=ItemType::VOID;
	else if (s=="BOOL") x=ItemType::BOOL;
	else if (s=="CHAR") x=ItemType::CHAR;
	else if (s=="INT") x=ItemType::INT;
	else if (s=="REAL") x=ItemType::REAL;
	else x=ItemType::ERROR;
	return IN;
}
ostream& operator<<(ostream& OUT,const ItemType& x)
{
	switch (x)
	{
	case ItemType::VOID:
		OUT<<"VOID";
		break;
	case ItemType::BOOL:
		OUT<<"BOOL";
		break;
	case ItemType::CHAR:
		OUT<<"CHAR";
		break;
	case ItemType::INT:
		OUT<<"INT";
		break;
	case ItemType::REAL:
		OUT<<"REAL";
		break;
	default:
		OUT<<"ERROR";
		break;
	}
	return OUT;
}
//add identify with format SymbolTableItem
//return 0 if success; otherwise failure
int SymbolTableBlock::AddItem(const SymbolTableItem &x)
{
	if (this->table.find(x)!=this->table.end()) return -1;
	this->table.insert(x);return 0;
}

bool isadapt(const std::vector<SymbolTablePara> &A,const std::vector<SymbolTablePara> &B)
{
	assert(A.size()==B.size());
	for (int i=0;i<A.size();i++) if (A[i].is_var() && !B[i].is_var()) return false;
	return true;
}
//find identify with format SymbolTableItem
//do not care about para.info_
//return if exist
bool SymbolTableBlock::Query(SymbolTableItem &x)
{
	for (std::shared_ptr<SymbolTableBlock> nw=std::make_shared<SymbolTableBlock>(*this);nw;nw=nw->father)
	{
		auto temp=nw->table.find(x);
		if (temp!=nw->table.end())
		{
			if (!isadapt(x.para(),temp->para())) continue;
			x=*temp;
			return true;
		}
	}
	return false;
}
std::shared_ptr<SymbolTableBlock> SymbolTableBlock::getfather(){return father;}
void SymbolTableBlock::linktofather(std::shared_ptr<SymbolTableBlock> tp){father=tp;}

//create a new block of SymbolTable from father block; return the new block
std::shared_ptr<SymbolTableBlock>  Locate(std::shared_ptr<SymbolTableBlock> father)
{
	std::shared_ptr<SymbolTableBlock> res(new SymbolTableBlock());
	res->linktofather(father);return res;
}

//delete the block of SymbolTable and return the father block
std::shared_ptr<SymbolTableBlock> Relocate(std::shared_ptr<SymbolTableBlock> to_del)
{
	std::shared_ptr<SymbolTableBlock> res=to_del->getfather();
	to_del.reset();return res;
}

