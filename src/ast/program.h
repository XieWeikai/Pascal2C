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

    // IdList -> id | IdList , id
    // eg. a, b, c
    class IdList
    {
    public:
        const string &operator[](const int &index) const { return id_list_[index]; }

        const int Size() const { return id_list_.size(); }

        void AddId(const string &id) { id_list_.push_back(id); }

        const string ToString(const int &level) const;

    private:
        vector<string> id_list_;
    };

    // Type -> (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR) | array [Period] of (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR)
    // Period -> digits ... digits | period , digits ... digits
    // eg. integer; real; boolean; char; array [1..10] of integer; array [1..10, 20..30] of real
    class Type
    {
    public:
        // digits_1 is the lower bound, digits_2 is the upper bound
        // e.g. 1..10
        struct Period
        {
            int digits_1;
            int digits_2;
        };

        Type(const bool &is_array) : is_array_(is_array) {}

        Type(const bool &is_array, const int &basic_type)
            : is_array_(is_array), basic_type_(basic_type) {}

        const bool &is_array() const { return is_array_; }

        const int &basic_type() const { return basic_type_; }

        const vector<Period> &periods() const { return periods_; }

        void AddPeriod(const int &digits_1, const int &digits_2)
        {
            periods_.push_back({digits_1, digits_2});
        }

        const string ToString(const int &level) const;

    private:
        bool is_array_;
        int basic_type_;
        vector<Period> periods_; // can be empty
    };

    // Parameter -> var_parameter | value_parameter
    // var_parameter -> var value_parameter
    // value_parameter -> id_list : (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR)
    // eg. a, b : integer; var c, d : real
    class Parameter
    {
    public:
        Parameter(const bool &is_var, shared_ptr<IdList> id_list, const int &type)
            : is_var_(is_var), id_list_(std::move(id_list)), type_(type) {}

        const bool &is_var() const { return is_var_; }

        const shared_ptr<IdList> &id_list() const { return id_list_; }

        const int &type() const { return type_; }

        const string ToString(const int &level) const;

    private:
        bool is_var_;
        shared_ptr<IdList> id_list_;
        int type_;
    };

    // ConstDeclaration -> id= (IntegerValue | RealValue | UnaryExpr | CharValue)
    // eg. a=1, b=2.0, c=-1, d='a'
    class ConstDeclaration
    {
    public:
        ConstDeclaration(const string &id, shared_ptr<Expression> const_value)
            : id(id), const_value(std::move(const_value)) {}

        const string &id() const { return id; }

        const shared_ptr<Expression> &const_value() const { return const_value; }

        const string ToString(const int &level) const;

    private:
        string id;
        shared_ptr<Expression> const_value; // IntegerValue | RealValue | UnaryExpr | CharValue
    };

    // VarDeclaration -> IdList : Type
    // eg. a, b, c : integer; d, e : array [1..10] of integer
    class VarDeclaration
    {
    public:
        VarDeclaration(shared_ptr<IdList> id_list, shared_ptr<Type> type)
            : id_list(std::move(id_list)), type(std::move(type)) {}

        const shared_ptr<IdList> &id_list() const { return id_list; }

        const shared_ptr<Type> &type() const { return type; }

        const string ToString(const int &level) const;

    private:
        shared_ptr<IdList> id_list;
        shared_ptr<Type> type;
    };

    // SubprogramHead -> function id (EMPTY | parameters) : (TOK_INTEGER | TOK_REAL | TOK_BOOLEAN | TOK_CHAR) | procedure id (EMPTY | parameters)
    // parameters -> Parameter | parameters ; Parameter
    // eg. function f(a, b : integer) : integer; procedure p(var c, d : real)
    class SubprogramHead
    {
    public:
        SubprogramHead(const string &id, const int &return_type = -1)
            : id_(id), return_type_(return_type) {}

        const string &id() const { return id_; }

        const int &return_type() const { return return_type_; }

        const vector<shared_ptr<Parameter>> &parameters() const { return parameters_; }

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
        const vector<shared_ptr<ConstDeclaration>> &const_declarations() const { return const_declarations_; }

        const vector<shared_ptr<VarDeclaration>> &var_declarations() const { return var_declarations_; }

        const vector<shared_ptr<Statement>> &statement_list() const { return statement_list_; }

        void AddConstDeclaration(shared_ptr<ConstDeclaration> const_declaration)
        {
            const_declarations_.push_back(std::move(const_declaration));
        }

        void AddVarDeclaration(shared_ptr<VarDeclaration> var_declaration)
        {
            var_declarations_.push_back(std::move(var_declaration));
        }

        void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        const string ToString(const int &level) const;

    private:
        vector<shared_ptr<ConstDeclaration>> const_declarations_; // can be empty
        vector<shared_ptr<VarDeclaration>> var_declarations_;     // can be empty
        vector<shared_ptr<Statement>> statement_list_;
    };

    // Subprogram -> SubprogramHead ; SubprogramBody
    class Subprogram
    {
    public:
        Subprogram(shared_ptr<SubprogramHead> subprogram_head, shared_ptr<SubprogramBody> subprogram_body)
            : subprogram_head(std::move(subprogram_head)), subprogram_body(std::move(subprogram_body)) {}

        const shared_ptr<SubprogramHead> &subprogram_head() const { return subprogram_head; }

        const shared_ptr<SubprogramBody> &subprogram_body() const { return subprogram_body; }

        const string ToString(const int &level) const;

    private:
        shared_ptr<SubprogramHead> subprogram_head;
        shared_ptr<SubprogramBody> subprogram_body;
    };

    // ProgramHead -> program id(IdList) | program id
    // eg. program f(a, b); program f
    class ProgramHead
    {
    public:
        ProgramHead(const string &id, shared_ptr<IdList> id_list)
            : id_(id), id_list_(std::move(id_list)) {}

        explicit ProgramHead(const string &id) : id_(id) {}

        const string &id() const { return id_; }

        const shared_ptr<IdList> &id_list() const { return id_list_; }

        const bool &HasIdList() const { return id_list_ != nullptr; }

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
        const vector<shared_ptr<ConstDeclaration>> &const_declarations() const { return const_declarations_; }

        const vector<shared_ptr<VarDeclaration>> &var_declarations() const { return var_declarations_; }

        const vector<shared_ptr<Subprogram>> &subprogram_declarations() const { return subprogram_declarations_; }

        const vector<shared_ptr<Statement>> &statement_list() const { return statement_list_; }

        void AddConstDeclaration(shared_ptr<ConstDeclaration> const_declaration)
        {
            const_declarations_.push_back(std::move(const_declaration));
        }

        void AddVarDeclaration(shared_ptr<VarDeclaration> var_declaration)
        {
            var_declarations_.push_back(std::move(var_declaration));
        }

        void AddSubprogram(shared_ptr<Subprogram> subprogram)
        {
            subprogram_declarations_.push_back(std::move(subprogram));
        }

        void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        const string ToString(const int &level) const;

    private:
        vector<shared_ptr<ConstDeclaration>> const_declarations_; // can be empty
        vector<shared_ptr<VarDeclaration>> var_declarations_;     // can be empty
        vector<shared_ptr<Subprogram>> subprogram_declarations_;  // can be empty
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
