#ifndef PASCAL2C_PROGRAM_H
#define PASCAL2C_PROGRAM_H

#include <memory>
#include <vector>
#include <string>

#include "ast.h"
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
    //
    // eg. a, b, c
    class IdList : public Ast
    {
    public:
        // param:
        //     id is the identifier
        // return:
        //     the id at the index
        inline const string &operator[](const int index) const { return id_list_[index]; }

        // return:
        //     the number of identifiers
        inline const int Size() const { return id_list_.size(); }

        inline void AddId(const string &id) { id_list_.push_back(id); }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        vector<string> id_list_; // a list of identifiers, eg. a, b, c
    };

    // Type -> (TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE) | array [Period] of (TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE)
    // Period -> digits ... digits | period , digits ... digits
    //
    // eg. integer, real, boolean, char
    // eg. array [1..10] of integer, array [1..10, 20..30] of real
    class Type : public Ast
    {
    public:
        // digits_1 is the lower bound, digits_2 is the upper bound
        //
        // e.g. 1..10
        struct Period
        {
            int digits_1;
            int digits_2;
        };

        // param:
        //     is_array is true if the type is array type
        Type(const int line, const int column, const bool is_array) : Ast(line, column), is_array_(is_array) {}

        // param:
        //     is_array is true if the type is array type
        //     basic_type is the basic type of the array, eg. integer, real, boolean, char
        Type(const int line, const int column, const bool is_array, const int basic_type)
            : Ast(line, column), is_array_(is_array), basic_type_(basic_type) {}

        inline const bool &is_array() const { return is_array_; }

        inline const int &basic_type() const { return basic_type_; }

        inline const vector<Period> &periods() const { return periods_; }

        inline void AddPeriod(const Period &period) { periods_.push_back(period); }

        inline void set_basic_type(const int basic_type) { basic_type_ = basic_type; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        bool is_array_;          // true if the type is array type
        int basic_type_;         // TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE from lexer, eg. integer, real, boolean, char
        vector<Period> periods_; // can be empty, eg. [1..10, 20..30]
    };

    // Parameter -> var_parameter | value_parameter
    // var_parameter -> var value_parameter
    // value_parameter -> id_list : (TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE)
    //
    // eg. a, b : integer
    // eg. var c, d : real
    class Parameter : public Ast
    {
    public:
        // param:
        //     is_var is true if the parameter is var parameter
        //     id_list is a list of identifiers
        //     type is the type of the identifiers
        Parameter(const int line, const int column, const bool is_var, shared_ptr<IdList> id_list, const int type)
            : Ast(line, column), is_var_(is_var), id_list_(std::move(id_list)), type_(type) {}

        inline const bool &is_var() const { return is_var_; }

        inline const shared_ptr<IdList> &id_list() const { return id_list_; }

        inline const int &type() const { return type_; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        bool is_var_;                // true if the parameter is var parameter
        shared_ptr<IdList> id_list_; // a list of identifiers, eg. a, b
        int type_;                   // TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE from lexer, eg. integer, real, boolean, char
    };

    // ConstDeclaration -> id= (IntegerValue | RealValue | UnaryExpr | CharValue)
    //
    // eg. a=1, b=2.0, c=-1, d='a'
    class ConstDeclaration : public Ast
    {
    public:
        // param:
        //     id is the identifier
        //     const_value is the value of the identifier
        ConstDeclaration(const int line, const int column, const string &id, shared_ptr<Expression> const_value)
            : Ast(line, column), id_(id), const_value_(std::move(const_value)) {}

        inline const string &id() const { return id_; }

        inline const shared_ptr<Expression> &const_value() const { return const_value_; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        string id_;                          // the identifier, eg. a
        shared_ptr<Expression> const_value_; // IntegerValue | RealValue | UnaryExpr | CharValue from expr.h, eg. 1, 2.0, -1, 'a'
    };

    // VarDeclaration -> IdList : Type
    //
    // eg. a, b, c : integer
    // eg. d, e : array [1..10] of integer
    class VarDeclaration : public Ast
    {
    public:
        // param:
        //     id_list is a list of identifiers
        //     type is the type of the identifiers
        VarDeclaration(const int line, const int column, shared_ptr<IdList> id_list, shared_ptr<Type> type)
            : Ast(line, column), id_list_(std::move(id_list)), type_(std::move(type)) {}

        inline const shared_ptr<IdList> &id_list() const { return id_list_; }

        inline const shared_ptr<Type> &type() const { return type_; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        shared_ptr<IdList> id_list_; // a list of identifiers, eg. a, b, c
        shared_ptr<Type> type_;      // the type of the identifiers, eg. integer
    };

    // SubprogramHead -> function id (ε | parameters) : (TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE) | procedure id (ε | parameters)
    // parameters -> Parameter | parameters ; Parameter
    //
    // eg. function f(a, b : integer) : integer
    // eg. procedure p(var c, d : real)
    class SubprogramHead : public Ast
    {
    public:
        // param:
        //     id is the name of the subprogram
        //     return_type is the return type of the subprogram, -1 means procedure
        SubprogramHead(const int line, const int column, const string &id, const int return_type = -1)
            : Ast(line, column), id_(id), return_type_(return_type) {}

        inline const string &id() const { return id_; }

        // return:
        //     the return type of the subprogram, -1 means procedure
        inline const int &return_type() const { return return_type_; }

        inline const vector<shared_ptr<Parameter>> &parameters() const { return parameters_; }

        inline void AddParameter(shared_ptr<Parameter> parameter) { parameters_.push_back(std::move(parameter)); }

        inline void set_return_type(const int return_type) { return_type_ = return_type; }

        // return:
        //     true if the subprogram is a function
        inline const bool is_function() const { return return_type_ != -1; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        string id_;                                // name of the subprogram, eg. f, p
        int return_type_;                          // -1 means procedure,TOK_INTEGER_TYPE | TOK_REAL_TYPE | TOK_BOOLEAN_TYPE | TOK_CHAR_TYPE ,eg. integer, real
        vector<shared_ptr<Parameter>> parameters_; // can be empty, eg. a, b : integer
    };

    // SubprogramBody -> (const const_declarations | ε)
    //                   (var var_declarations | ε)
    //                   (statement_list | ε)
    // const_declarations -> id=ConstValue | const_declarations ; id=ConstValue
    // var_declarations -> IdList : Type | var_declarations ; IdList : Type
    // statement_list -> statement | statement_list ; statement
    //
    // eg. const a = 1; b = 2; var c, d : integer; begin end
    class SubprogramBody : public Ast
    {
    public:
        inline const vector<shared_ptr<ConstDeclaration>> &const_declarations() const
        {
            return const_declarations_;
        }

        inline const vector<shared_ptr<VarDeclaration>> &var_declarations() const { return var_declarations_; }

        inline const vector<shared_ptr<Statement>> &statement_list() const { return statement_list_; }

        inline void AddConstDeclaration(shared_ptr<ConstDeclaration> const_declaration)
        {
            const_declarations_.push_back(std::move(const_declaration));
        }

        inline void AddVarDeclaration(shared_ptr<VarDeclaration> var_declaration)
        {
            var_declarations_.push_back(std::move(var_declaration));
        }

        inline void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        vector<shared_ptr<ConstDeclaration>> const_declarations_; // can be empty, eg. const a = 1; b = 2;
        vector<shared_ptr<VarDeclaration>> var_declarations_;     // can be empty, eg. var c, d : integer;
        vector<shared_ptr<Statement>> statement_list_;            // can be empty, eg. begin end
    };

    // Subprogram -> SubprogramHead ; SubprogramBody
    //
    // eg. function f(a, b : integer) : integer; begin end
    // eg. procedure p(var c, d : real); begin end
    class Subprogram : public Ast
    {
    public:
        // param:
        //     subprogram_head is the head of the subprogram
        //     subprogram_body is the body of the subprogram
        Subprogram(const int line, const int column, shared_ptr<SubprogramHead> subprogram_head, shared_ptr<SubprogramBody> subprogram_body)
            : Ast(line, column), subprogram_head_(std::move(subprogram_head)), subprogram_body_(std::move(subprogram_body)) {}

        inline const shared_ptr<SubprogramHead> &subprogram_head() const { return subprogram_head_; }

        inline const shared_ptr<SubprogramBody> &subprogram_body() const { return subprogram_body_; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        shared_ptr<SubprogramHead> subprogram_head_; // eg. function f(a, b : integer) : integer;
        shared_ptr<SubprogramBody> subprogram_body_; // eg. begin ... end;
    };

    // ProgramHead -> program id(IdList) | program id
    //
    // eg. program f(a, b)
    // eg. program f
    class ProgramHead : public Ast
    {
    public:
        // param:
        //     id is the program name
        //     id_list is the parameters of the program
        ProgramHead(const int line, const int column, const string &id, shared_ptr<IdList> id_list)
            : Ast(line, column), id_(id), id_list_(std::move(id_list)) {}

        // param:
        //     id is the program name
        ProgramHead(const int line, const int column, const string &id) : Ast(line, column), id_(id) {}

        inline const string &id() const { return id_; }

        inline const shared_ptr<IdList> &id_list() const { return id_list_; }

        // check if the program has parameters
        // return:
        //     true if the program has parameters
        inline const bool HasIdList() const { return id_list_ != nullptr; }

        inline const string &id() const { return id_; }

        inline const shared_ptr<IdList> &id_list() const { return id_list_; }

        // check if the program has parameters
        // return:
        //     true if the program has parameters
        inline const bool HasIdList() const { return id_list_ != nullptr; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        string id_;                  // program name, eg. f
        shared_ptr<IdList> id_list_; // parameters, can be empty, eg. (a, b)
    };

    // ProgramBody -> (const const_declarations | ε)
    //                (var var_declarations | ε)
    //                (subprogram_declarations | ε)
    //                (statement_list | ε)
    // const_declarations -> ConstDeclaration | (const_declarations ; ConstDeclaration)
    // var_declarations -> VarDeclaration | var_declarations ; VarDeclaration
    // subprogram_declarations -> Subprogram | subprogram_declarations ; Subprogram
    // statement_list -> statement | statement_list ; statement
    //
    // eg. const a = 1; b = 2; var c, d : integer; procedure p; begin end; begin end
    class ProgramBody : public Ast
    {
    public:
        inline const vector<shared_ptr<ConstDeclaration>> &const_declarations() const { return const_declarations_; }

        inline const vector<shared_ptr<VarDeclaration>> &var_declarations() const { return var_declarations_; }

        inline const vector<shared_ptr<Subprogram>> &subprogram_declarations() const { return subprogram_declarations_; }

        inline const vector<shared_ptr<Statement>> &statement_list() const { return statement_list_; }

        inline void AddConstDeclaration(shared_ptr<ConstDeclaration> const_declaration)
        {
            const_declarations_.push_back(std::move(const_declaration));
        }

        inline void AddVarDeclaration(shared_ptr<VarDeclaration> var_declaration)
        {
            var_declarations_.push_back(std::move(var_declaration));
        }

        inline void AddSubprogram(shared_ptr<Subprogram> subprogram)
        {
            subprogram_declarations_.push_back(std::move(subprogram));
        }

        inline void AddStatement(shared_ptr<Statement> statement) { statement_list_.push_back(std::move(statement)); }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        vector<shared_ptr<ConstDeclaration>> const_declarations_; // can be empty, eg. const a = 1; b = 2;
        vector<shared_ptr<VarDeclaration>> var_declarations_;     // can be empty, eg. var c, d : integer;
        vector<shared_ptr<Subprogram>> subprogram_declarations_;  // can be empty, eg. procedure p; begin end;
        vector<shared_ptr<Statement>> statement_list_;            // can be empty, eg. begin end
    };

    // Program -> ProgramHead; ProgramBody.
    //
    // eg. program f(a, b); var a, b; begin a := 1; b := 2; end.
    class Program : public Ast
    {
    public:
        // param:
        //     program_head is the shared pointer of ProgramHead
        //     program_body is the shared pointer of ProgramBody
        Program(const int line, const int column, shared_ptr<ProgramHead> program_head, shared_ptr<ProgramBody> program_body)
            : Ast(line, column), program_head_(std::move(program_head)), program_body_(std::move(program_body)) {}

        inline const shared_ptr<ProgramHead> &program_head() const { return program_head_; }

        inline const shared_ptr<ProgramBody> &program_body() const { return program_body_; }

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        // return:
        //     a string represents the statement
        const string ToString(const int level) const;

    private:
        shared_ptr<ProgramHead> program_head_; // eg. program f(a, b)
        shared_ptr<ProgramBody> program_body_; // eg. var a, b; begin a := 1; b := 2; end.
    };
}

#endif // PASCAL2C_PROGRAM_H
