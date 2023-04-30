#pragma once

#include <map>
#include <set>
#include <vector>
#include <memory>
#include "../ast/ast.h"

namespace symbol_table{
	enum ItemType{
        ERROR,
        VOID,
        BOOL,
        CHAR,
		INT,
		REAL
	};
	
	class SymbolTablePara{
	public:
		SymbolTablePara(ItemType type, bool is_var,std::string info=""):
			type_(type), is_var_(is_var), info_(info){}
			
		ItemType type()const{return type_;}
        bool is_var()const{return is_var_;}
        std::string info()const{return info_;}
        friend bool operator<(const SymbolTablePara &A,const SymbolTablePara &B){return A.type()<B.type();};
        friend bool operator==(const SymbolTablePara &A,const SymbolTablePara &B){return A.type()==B.type();};
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
		
        std::string name()const{return name_;}
        ItemType type()const{return type_;}
        bool is_var()const{return is_var_;}
        bool is_func()const{return is_func_;}
		std::vector<SymbolTablePara> para()const{return para_;}
        friend bool operator<(const SymbolTableItem &A,const SymbolTableItem &B){return A.name()==B.name() ? A.para()<B.para() : A.name()<B.name();};
        friend bool operator==(const SymbolTableItem &A,const SymbolTableItem &B){return A.name()==B.name() && A.para()==B.para();};
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
        //do not care about para.info_
        //return if exist
        bool Query(SymbolTableItem &x);
        
        std::shared_ptr<SymbolTableBlock> getfather();
        void linktofather(std::shared_ptr<SymbolTableBlock> x);
    private:
        std::shared_ptr<SymbolTableBlock> father;
        std::set<SymbolTableItem> table;
    };

    //create a new block of SymbolTable from father block; return the new block
    std::shared_ptr<SymbolTableBlock>  Locate(std::shared_ptr<SymbolTableBlock> father);
    
    //delete the block of SymbolTable and return the father block
    std::shared_ptr<SymbolTableBlock> Relocate(std::shared_ptr<SymbolTableBlock> to_del);
    
}
