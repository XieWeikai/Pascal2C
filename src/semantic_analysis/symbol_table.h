#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include "../ast/ast.h"
#include "errors.h"
using std::ostream;
using std::istream;
namespace symbol_table{
	enum ItemType{
        ERROR,
        VOID,
        BOOL,
        CHAR,
		INT,
		REAL,
        STRING
	};
	static istream& operator>>(istream& IN,ItemType& x)
    {
        std::string s;IN>>s;
        if (s=="VOID") x=ItemType::VOID;
        else if (s=="BOOL") x=ItemType::BOOL;
        else if (s=="CHAR") x=ItemType::CHAR;
        else if (s=="INT") x=ItemType::INT;
        else if (s=="REAL") x=ItemType::REAL;
        else if (s=="STRING") x=ItemType::STRING;
        else x=ItemType::ERROR;
        return IN;
    }
	static ostream& operator<<(ostream& OUT,const ItemType& x)
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
        case ItemType::STRING:
            OUT<<"STRING";
            break;
        default:
            OUT<<"ERROR";
            break;
        }
        return OUT;
    }
    class MegaType
    {
        public:
        MegaType():pointer_(std::vector<std::string>()){}
        MegaType(symbol_table::ItemType typein):
            type_(typein),pointer_(std::vector<std::string>()){}
        MegaType(symbol_table::ItemType typein,std::vector<std::string> pointerin):
            type_(typein),pointer_(pointerin){}
        symbol_table::ItemType type()const{return type_;}
        std::vector<std::string> pointer()const{return pointer_;}
        void settype(symbol_table::ItemType newtype){type_=newtype;}
        void setpointer(std::vector<std::string> pointerin){pointer_=pointerin;}
        void addpointer(std::string inp){pointer_.push_back(inp);}
        friend istream& operator>>(istream& IN,MegaType& x)
        {
            IN>>x.type_;
            for(int i=0;i<x.pointer_.size();i++)
                IN>>x.pointer_[i];
            return IN;
        }
        friend ostream& operator<<(ostream& OUT,const MegaType& x)
        {
            OUT<<x.type_;
            if(x.pointer_.size()!=0)    OUT<<"[";
            for (int i=0;i<x.pointer_.size();i++)
            {
                if(i!=0)    OUT<<",";
                OUT<<x.pointer_[i];
            }
            if(x.pointer_.size()!=0)    OUT<<"]";
            return OUT;
        }
        MegaType operator=(const ItemType &x){return MegaType(x);}
        friend bool operator<(const MegaType &A,const MegaType &B){return A.type_==B.type_ ? A.pointer_<B.pointer_ :A.type_<B.type_;}
        friend bool operator==(const MegaType &A,const MegaType &B){return A.type_==B.type_ && A.pointer_==B.pointer_;}
        friend bool operator!=(const MegaType &A,const MegaType &B){return !(A==B);}
        private:
        std::vector<std::string> pointer_;
        symbol_table::ItemType type_;
    };
	class SymbolTablePara{
	public:
		SymbolTablePara(){}
		SymbolTablePara(ItemType type, bool is_var,std::string info=""):
			type_(type), is_var_(is_var), info_(info){}
		SymbolTablePara(MegaType type, bool is_var,std::string info=""):
			type_(type), is_var_(is_var), info_(info){}	
		MegaType type()const{return type_;}
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
		MegaType type_;
        bool is_var_;
        std::string info_;
	};
	
    //item of symbol table
    class SymbolTableItem{
    public:
        SymbolTableItem(){}
		SymbolTableItem(MegaType type, std::string name, bool is_var, bool is_func, std::vector<SymbolTablePara> para):
			type_(type), name_(name), is_var_(is_var), is_func_(is_func), para_(para){}
		SymbolTableItem(ItemType type, std::string name, bool is_var, bool is_func, std::vector<SymbolTablePara> para):
			type_(type), name_(name), is_var_(is_var), is_func_(is_func), para_(para){}
		
        std::string name()const{return name_;}
        MegaType type()const{return type_;}
        void settype(MegaType newtype){type_=newtype;}
        void setIsVar(){is_var_=!is_var_;}
        void setIsFunc(){is_func_=!is_func_;}
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
		MegaType type_;
        std::string name_;
        bool is_var_;
        bool is_func_;
		std::vector<SymbolTablePara> para_;
    };
	
    class SymbolTableBlock{
    public:
        //add identify with format SymbolTableItem
        //return 0 if success; otherwise failure
        saERRORS::ERROR_TYPE AddItem(const SymbolTableItem &x);
        
        //find identify with format SymbolTableItem
        //do not care about para.info_
        //return if exist
        saERRORS::ERROR_TYPE Query(SymbolTableItem &x);
        
        std::shared_ptr<SymbolTableBlock> getfather();
        friend ostream& operator<<(ostream& OUT,const SymbolTableBlock& x)
        {
            for (auto &temp:x.table) for(auto &res:temp.second) OUT<<res<<std::endl;
            return OUT;
        }
        void Locate(std::shared_ptr<SymbolTableBlock> nowfather)
        {
            father=nowfather;
        }
    private:
        std::shared_ptr<SymbolTableBlock> father;
        std::map<std::string,bool> name_domain;//mark name belong func or variable;
        std::map<std::string,std::set<SymbolTableItem> > table;
    }; 
}