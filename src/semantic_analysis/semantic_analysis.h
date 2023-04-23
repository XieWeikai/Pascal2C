#include <vector>
#include "symbol_table.h"

namespace analysiser{
    std::vector<std::string> blockNames;//memary name of the latest block
    std::string nowblockName; 
    //manage SymbolTableBlock
    class nameTable{
    public:
        //add new SymbolTableBlock with name
        //return success or failure
        bool Add(std::string name, std::shared_ptr<symbol_table::SymbolTableBlock> block);
        //find SymbolTableBlock named name
        //return success or failure. if success, put target on ansblock
        bool Query(std::string name, std::shared_ptr<symbol_table::SymbolTableBlock> &ansblock);
    private:
        std::map<std::string, std::shared_ptr<symbol_table::SymbolTableBlock> > table_;
   };
    
}//end namespace analysiser