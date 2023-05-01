#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include "../ast/ast.h"
using std::ostream;
using std::istream;
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
		SymbolTablePara(){}
		SymbolTablePara(ItemType type, bool is_var,std::string info=""):
			type_(type), is_var_(is_var), info_(info){}
			
		ItemType type()const{return type_;}
        bool is_var()const{return is_var_;}
        std::string info()const{return info_;}
        friend bool operator<(const SymbolTablePara &A,const SymbolTablePara &B){return A.type_<B.type_;};
        friend bool operator==(const SymbolTablePara &A,const SymbolTablePara &B){return A.type_==B.type_;};
        /*
		input format:
			is_var type info
		info="NULL" means info is empty.
        */
        friend istream& operator>>(istream& IN,SymbolTablePara& x)
        {
        	IN>>x.is_var_>>x.type_>>x.info_;
        	if (x.info_=="NULL") x.info_="";
        	return IN;
        }
        /*
		output format:
			[is_var type info]
        */
        friend ostream& operator<<(ostream& OUT,const SymbolTablePara& x)
        {
            OUT<<"["<<(x.is_var_?"var ":"const ")<<x.type_<<" "<<x.info_<<"]";
            return OUT;
        }
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
        friend bool operator<(const SymbolTableItem &A,const SymbolTableItem &B){return A.name_==B.name_ ? A.para_<B.para_ : A.name_<B.name_;};
        friend bool operator==(const SymbolTableItem &A,const SymbolTableItem &B){return A.name_==B.name_ && A.para_==B.para_;};
        /*
		input format:
			is_var is_func type name num_of_paras para1 para2 ... 
        */
        friend istream& operator>>(istream& IN,SymbolTableItem& x)
        {
        	IN>>x.is_var_>>x.is_func_>>x.type_>>x.name_;
        	int k;IN>>k;
        	while (k--){SymbolTablePara temp;IN>>temp;x.para_.push_back(temp);}
        	return IN;
        }
        /*
		output format:
			is_var is_func type name[para1][para2]...
        */
        friend ostream& operator<<(ostream& OUT,const SymbolTableItem& x)
        {
            OUT<<(x.is_var_?"var ":"const ")<<(x.is_func_?"func ":"num ")<<x.type_<<" "<<x.name_;
            for (auto &temp:x.para_) OUT<<temp;
            return OUT;
        }
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
        friend ostream& operator<<(ostream& OUT,const SymbolTableBlock& x)
        {
            for (auto &temp:x.table) OUT<<temp<<std::endl;
            return OUT;
        }
    private:
        std::shared_ptr<SymbolTableBlock> father;
        std::set<SymbolTableItem> table;
    };

    //create a new block of SymbolTable from father block; return the new block
    std::shared_ptr<SymbolTableBlock>  Locate(std::shared_ptr<SymbolTableBlock> father);
    
    //delete the block of SymbolTable and return the father block
    std::shared_ptr<SymbolTableBlock> Relocate(std::shared_ptr<SymbolTableBlock> to_del);
    
}
