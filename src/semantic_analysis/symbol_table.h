#include <map>
#include <set>
#include "../ast/ast.h"

namespace symbol_table{
    class SymbolTableItem;
	enum ItemType{
		INT,
		REAL,
		CHAR,
		BOOL,
        VOID,
	};
	
	class SymbolTablePara{
	public:
		SymbolTablePara(ItemType type, bool is_var,std::string info=""):
			type_(type), is_var_(is_var), info_(info){}
			
		bool operator==(const SymbolTablePara &x);
	private:
		ItemType type_;
        bool is_var_;
        std::string info_;
	};
	
    //item of symbol table
    class SymbolTableItem{
    public:
		SymbolTableItem(ItemType type, std::string name, bool is_var, bool is_func, std::vector<SymbolTablePara> para):
			type_(type), name_(name), is_var_(is_var), is_func_(is_func), para_(para){}
		
        std::string name(){return name_;}
		bool operator==(const SymbolTableItem &x);
    private:
		ItemType type_;
        std::string name_;
        bool is_var_;
        bool is_func_;
		std::vector<SymbolTablePara> para_;
    };
	
    class SymbolTableBlock{
    public:
        //add identify with format SymbolTableItem
        //return 0 if success; otherwise failure
        int AddItem(const SymbolTableItem &x);
        
        //find identify with format SymbolTableItem
        //return if exist
        bool Query(SymbolTableItem &x);
        
    private:
        std::shared_ptr<SymbolTableBlock> father;
        std::map<std::string,int> idlist;
        std::set<SymbolTableBlock> table;
    };

    //create a new block of SymbolTable from father block; return the new block
    std::shared_ptr<SymbolTableBlock>  Locate(std::shared_ptr<SymbolTableBlock> father);
    
    //delete the block of SymbolTable and return the father block
    std::shared_ptr<SymbolTableBlock> Relocate(std::shared_ptr<SymbolTableBlock> to_del);
    
}
