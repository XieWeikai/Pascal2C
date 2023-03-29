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
}//end namespace analysiser