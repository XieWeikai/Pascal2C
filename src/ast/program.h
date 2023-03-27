#ifndef PASCAL2C_PROGRAM_H
#define PASCAL2C_PROGRAM_H

#include <memory>
#include <vector>
#include <string>

#include "expr.h"
#include "statement.h"

namespace pascal2c::ast
{
    template <typename Tp>
    using shared_ptr = ::std::shared_ptr<Tp>;
    template <typename Tp>
    using vector = ::std::vector<Tp>;
    using string = ::std::string;

    // ConstDeclaration -> id= (IntegerValue | RealValue | UnaryExpr | CharValue)
    struct ConstDeclaration
    {
        string id;
        shared_ptr<Expression> const_value; // IntegerValue | RealValue | UnaryExpr | CharValue
    };

    // VarDeclaration -> IdList : Type
    struct VarDeclaration
    {
        shared_ptr<IdList> id_list;
        shared_ptr<Type> type;
    };

    // Subprogram -> SubprogramHead ; SubprogramBody
    struct Subprogram
    {
        shared_ptr<SubprogramHead> subprogram_head;
        shared_ptr<SubprogramBody> subprogram_body;
    };

    // IdList -> id | IdList , id
    class IdList
    {
    public:
        void AddId(const string &id) { id_list_.push_back(id); }

        const string ToString(const int &level) const;

    private:
        vector<string> id_list_;
    };

    // Type -> (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR) | array [Period] of (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR)
    // Period -> digits ... digits | period , digits ... digits
    class Type
    {
    public:
        Type(const bool &is_array) : is_array_(is_array) {}

        Type(const bool &is_array, const int &basic_type)
            : is_array_(is_array), basic_type_(basic_type) {}

        void AddPeriod(const int &digits_1, const int &digits_2)
        {
            periods_.push_back({digits_1, digits_2});
        }

        const string ToString(const int &level) const;

    private:
        struct Period
        {
            int digits_1;
            int digits_2;
        };

        bool is_array_;
        int basic_type_;
        vector<Period> periods_; // can be empty
    };

    // Parameter -> var_parameter | value_parameter
    // var_parameter -> var value_parameter
    // value_parameter -> id_list : (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR)
    class Parameter
    {
    public:
        Parameter(const bool &is_var, shared_ptr<IdList> id_list, const int &type)
            : is_var_(is_var), id_list_(std::move(id_list)), type_(type) {}

        const string ToString(const int &level) const;

    private:
        bool is_var_;
        shared_ptr<IdList> id_list_;
        int type_;
    };

    // SubprogramHead -> function id (EMPTY | parameters) : (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR) | procedure id (EMPTY | parameters)
    // parameters -> Parameter | parameters ; Parameter
    class SubprogramHead
    {
    public:
        SubprogramHead(const string &id, const int &return_type = -1)
            : id_(id), return_type_(return_type) {}

        void AddParameter(shared_ptr<Parameter> parameter) { parameters_.push_back(std::move(parameter)); }

        void set_return_type(const int &return_type) { return_type_ = return_type; }

        const string ToString(const int &level) const;

    private:
        string id_;
        int return_type_;                          // -1 means procedure
        vector<shared_ptr<Parameter>> parameters_; // can be empty
    };

    // SubprogramBody -> (const const_declarations | EMPTY)
    //                   (var var_declarations | EMPTY)
    //                   statement_list
    // const_declarations -> id=ConstValue | const_declarations ; id=ConstValue
    // var_declarations -> IdList : Type | var_declarations ; IdList : Type
    // statement_list -> statement | statement_list ; statement
    class SubprogramBody
    {
    public:
        void AddConstDeclaration(const string &id, shared_ptr<Expression> const_value)
        {
            const_declarations_.push_back({id, std::move(const_value)});
        }

        void AddVarDeclaration(shared_ptr<IdList> id_list, shared_ptr<Type> type)
        {
            var_declarations_.push_back({std::move(id_list), std::move(type)});
        }

        void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        const string ToString(const int &level) const;

    private:
        vector<ConstDeclaration> const_declarations_; // can be empty
        vector<VarDeclaration> var_declarations_;     // can be empty
        vector<shared_ptr<Statement>> statement_list_;
    };

    // ProgramHead -> program id(IdList) | program id
    class ProgramHead
    {
    public:
        ProgramHead(const string &id, shared_ptr<IdList> id_list)
            : id_(id), id_list_(std::move(id_list)) {}

        explicit ProgramHead(const string &id) : id_(id) {}

        const string ToString(const int &level) const;

    private:
        string id_;
        shared_ptr<IdList> id_list_; // can be empty
    };

    // ProgramBody -> (const const_declarations | EMPTY)
    //                (var var_declarations | EMPTY)
    //                (subprogram_declarations | EMPTY)
    //                 statement_list
    // const_declarations -> ConstDeclaration | (const_declarations ; ConstDeclaration)
    // var_declarations -> VarDeclaration | var_declarations ; VarDeclaration
    // subprogram_declarations -> Subprogram | subprogram_declarations ; Subprogram
    // statement_list -> statement | statement_list ; statement
    class ProgramBody
    {
    public:
        void AddConstDeclaration(const string &id, shared_ptr<Expression> const_value)
        {
            const_declarations_.push_back({id, std::move(const_value)});
        }

        void AddVarDeclaration(shared_ptr<IdList> id_list, shared_ptr<Type> type)
        {
            var_declarations_.push_back({std::move(id_list), std::move(type)});
        }

        void AddSubprogram(shared_ptr<SubprogramHead> subprogram_head, shared_ptr<SubprogramBody> subprogram_body)
        {
            subprogram_declarations_.push_back({std::move(subprogram_head), std::move(subprogram_body)});
        }

        void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        const string ToString(const int &level) const;

    private:
        vector<ConstDeclaration> const_declarations_; // can be empty
        vector<VarDeclaration> var_declarations_;     // can be empty
        vector<Subprogram> subprogram_declarations_;  // can be empty
        vector<shared_ptr<Statement>> statement_list_;
    };

    // Program -> ProgramHead; ProgramBody.
    class Program
    {
    public:
        Program(shared_ptr<ProgramHead> program_head, shared_ptr<ProgramBody> program_body)
            : program_head_(std::move(program_head)), program_body_(std::move(program_body)) {}

        const string ToString(const int &level) const;

    private:
        shared_ptr<ProgramHead> program_head_;
        shared_ptr<ProgramBody> program_body_;
    };

}

#endif // PASCAL2C_PROGRAM_H
