#include "symbol_table.h"
#include <vector>
namespace analysiser{
    std::vector<std::string> blockNames;//memary name of the latest block
    std::string nowblockName; 
    bool Insert(const symbol_table::SymbolTableItem &x);
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
    nameTable table;
}//end namespace analysiser