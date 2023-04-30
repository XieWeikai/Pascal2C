#include <map>
#include <set>
#include "../ast/ast.h"
#include"symbol_table.h"

using namespace symbol_table;

bool operator<(const SymbolTablePara &A,const SymbolTablePara &B)
{
    return A.type()==B.type() ? A.is_var()<B.is_var() : A.type()<B.type();
}

bool operator==(const SymbolTablePara &A,const SymbolTablePara &B)
{
    return A.type()==B.type() && A.is_var()==B.is_var();
}

bool operator<(const SymbolTableItem &A,const SymbolTableItem &B)
{
    return A.name()==B.name() ? A.para()<B.para() : A.name()<B.name();
}

bool operator==(const SymbolTableItem &A,const SymbolTableItem &B)
{
    return A.name()==B.name() && A.para()==B.para();
}


//add identify with format SymbolTableItem
//return 0 if success; otherwise failure
int SymbolTableBlock::AddItem(const SymbolTableItem &x)
{
	if (this->table.find(x)!=this->table.end()) return -1;
	this->table.insert(x);return 0;
}

//find identify with format SymbolTableItem
//do not care about para.info_
//return if exist
bool SymbolTableBlock::Query(SymbolTableItem &x)
{
	auto temp=this->table.find(x);
	if (temp!=this->table.end())
	{
		x=*temp;
		return true;
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

