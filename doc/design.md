# 详细设计

## 词法分析

### 数据结构说明



### 函数、方法说明



### 算法描述



---

## 语法分析

### AST 树 

#### 功能描述

存储 AST 树，作为语法分析的输出，交给语义分析进一步处理

#### 接口描述（描述其他模块可能使用到的接口函数）

##### class Program

  | 描述       | 接口                                                 | 参数 | 返回值 | 异常 |
  | ---------- | ---------------------------------------------------- | ---- | ------ | ---- |
  | 获取程序头 | const shared_ptr\<ProgramHead> &program_head() const | 无   | 程序头 | 无   |

#### 数据结构定义（为类定义里的函数（除了get和set这类简单函数）和数据成员加上注释，直接复制过来）

为了完整表示源程序的所有信息，我们为pascal各种语法结构设计了相应的AST节点，各个类的定义均放在`pascal2c::ast`命名空间中，具体定义如下文所述

##### class Program
  AST 树根节点，存有指向 `ProgramHead`、`ProgramBody`的指针

```c++
    // Program -> ProgramHead; ProgramBody.
    class Program
    {
    public:
        // param:
        //     program_head is the shared pointer of ProgramHead
        //     program_body is the shared pointer of ProgramBody
        Program(shared_ptr<ProgramHead> program_head, shared_ptr<ProgramBody> program_body)
            : program_head_(std::move(program_head)), program_body_(std::move(program_body)) {}

        // for test use
        // param:
        //     level is the level of indentation that should be applied to the returned string
        const string ToString(const int &level) const;

    private:
        shared_ptr<ProgramHead> program_head_; //Program head
        shared_ptr<ProgramBody> program_body_; //Program body
    };
```

##### class Statement

该类表示一种pascal中的语句，为各种`Statement`的基类，定义如下

```c++
// base type of statement
class Statement {
public:
    // for test use
    // param:
    //     level is the level of indentation that should be applied to the returned string
    // return:
    //     a string represents the statement
    virtual std::string ToString(int level) const = 0;
    // to get exact statement type of the statement
    // return:
    //     exact type of statement
    virtual StatementType GetType() const = 0;
};
```

其中`StatementType`为枚举类型，其定义如下

```c++
// to identify what type a statement is
enum StatementType{
    ASSIGN_STATEMENT = 1,
    CALL_STATEMENT = 2,
    COMPOUND_STATEMENT = 3,
    IF_STATEMENT = 4,
    FOR_STATEMENT = 5,
};
```

##### class AssignStatement

该类表示一条pascal中的赋值语句，定义如下

```c++
// var_ := expr_
// var_ can be: var_name    var_name[expr_list]  func_id
class AssignStatement : public Statement {
public:
    // basic constructor
    // param:
    //     var is used to initialize the class member var_
    //     expr is used to initialize the class member expr_
    AssignStatement(std::shared_ptr<Variable> var,std::shared_ptr<Expression> expr) : var_(std::move(var)),expr_(
            std::move(expr)) {}

    inline StatementType GetType() const override { return ASSIGN_STATEMENT; }

    std::string ToString(int level) const override;
private:
    std::shared_ptr<Variable> var_;      // lhs of the assign statement
    std::shared_ptr<Expression> expr_;   // rhs of the assign statement
};
```

其中`Variable`类型和`Expression`类型放在后文解释。

##### class CallStatement

该类表示一条pascal中调用子程序(procedure或者function)的语句，定义如下

```c++
// represents procedure and function call
// name_ is procedure name or function name
// expr_list_ is the arguments of procedure or function
// expr_list_ can be empty
// write and read are treated as CallStatement as built-in function
class CallStatement : public Statement {
public:
    CallStatement(std::string name, vector<std::shared_ptr<Expression> > expr_list) :
            name_(std::move(name)), expr_list_(std::move(expr_list)) {}

    explicit CallStatement(std::string name) : name_(std::move(name)) {}

    inline StatementType GetType() const override { return CALL_STATEMENT; }

    std::string ToString(int level) const override;
private:
    std::string name_;   // procedure name or function name

    vector<std::shared_ptr<Expression> > expr_list_; // can be empty:  e.g. procedure_name;   func();
};
```

##### class CompoundStatement

该类表示pascal中以`begin`和`end`包围的一系列语句，定义如下

```c++
// a series of statements
// begin
//    statement1 ;
//    statement2 ;
//    ...
// end
class CompoundStatement: public Statement {
public:
    explicit CompoundStatement(vector<std::shared_ptr<Statement> > statements) : statements_(std::move(statements)) {}

    inline StatementType GetType() const override { return COMPOUND_STATEMENT; }

    std::string ToString(int level) const override;
private:
    vector<std::shared_ptr<Statement> > statements_;  // vector of statement
};
```

##### class  IfStatement

该类表示pascal中的`if`语句，定义如下

```c++
// if condition_ then
//      then_
// else
//      else_
class IfStatement : public Statement {
public:
    IfStatement(std::shared_ptr<Expression> cond,std::shared_ptr<Statement> then,
                std::shared_ptr<Statement> else_part)
                : condition_(std::move(cond)),then_(std::move(then)),else_(std::move(else_part)) {}

    inline StatementType GetType() const override { return IF_STATEMENT; }

    std::string ToString(int level) const override;
private:
    std::shared_ptr<Expression> condition_;    // condition expression
    std::shared_ptr<Statement>  then_;         // then part of if statement
    std::shared_ptr<Statement>  else_;         // else part of if statement, can be empty
};
```

##### class ForStatement

该类表示一条pascal中的`for`语句，定义如下

```c++
// for id_ := from_ to to_ do statement_
class ForStatement : public Statement {
public:
    ForStatement(std::shared_ptr<Expression> from,std::shared_ptr<Expression> to,
                 std::shared_ptr<Statement>  statement)
                 : from_(std::move(from)),to_(std::move(to)),statement_(std::move(statement)) {}

    inline StatementType GetType() const override { return FOR_STATEMENT; }

    std::string ToString(int level) const override;
private:
    std::string id_;
    std::shared_ptr<Expression> from_;
    std::shared_ptr<Expression> to_;
    std::shared_ptr<Statement>  statement_;
};
```

##### class Expression

该类表示pascal中一条合法的表达式，由于pascal中表达式有很多个部分构成、有很多种表达式，故统一用一个`Expression`类来做表达式的基类，该类定义如下

```c++
// base class for expression
class Expression {
public:
    // for test use
    // param:
    //     level is the level of indentation that should be applied to the returned string
    // return:
    //     a string represents the statement
    virtual std::string ToString(int level) const = 0;

    // get the exact type of the expression
    // return:
    //     one of ExprType
    virtual ExprType GetType() const = 0;
};
```

其中`ExprType`的定义如下

```c++
enum ExprType{
    INT = 0,
    REAL = 1,
    CHAR = 2,
    BOOLEAN = 3,
    VARIABLE = 4,
    CALL = 5,
    BINARY = 6,
    UNARY = 7,
};
```

##### class IntegerValue

单独的整数可以作为合法的pascal表达式，定义表示一个整数的节点如下

```c++
// leaf node of an expression
// this node represents an integer value in pascal
// e.g. 123  482
// value_ stores the value of the integer
class IntegerValue : public Expression {
public:
    explicit IntegerValue(int value) : value_(value) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return INT;}

private:
    int value_;
};
```

##### Class RealValue

单独的实数可以作为合法的pascal表达式，定义表示一个实数的节点如下

```c++
// leaf node of an expression
// this node represents a real value in pascal
// e.g. 12.34
// value_ stores the value of the real number
class RealValue : public Expression {
public:
    explicit RealValue(double value) : value_(value) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return REAL;}

private:
    double value_;
};
```

##### class CharValue

该类表示一个字符，形如`'a'`，表示该类型值的节点如下

```c++
// leaf node
// represent a boolean value
// e.g. true  false
// value_ store the value
class BooleanValue : public Expression {
public:
    explicit BooleanValue(bool value) : value_(value) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return BOOLEAN;}

private:
    bool value_;
};
```

##### class BooleanValue

该类表示一个`bool`值，可以为`true`或`false`，定义如下

```c++
// leaf node
// represent a boolean value
// e.g. true  false
// value_ store the value
class BooleanValue : public Expression {
public:
    explicit BooleanValue(bool value) : value_(value) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return BOOLEAN;}

private:
    bool value_;
};
```

##### class CallValue

该类表示调用一个函数产生的值，如`add(1,2)`，其中`add`为一个函数，返回值为整数。该类节点定义如下

```c++
// represent a function call
// e.g. add(3+4,5)
// params_ is a vector of expressions used as parameters of the function
// params_ can be empty
class CallValue : public Expression {
public:
    explicit CallValue(std::string func_name) : func_name_(std::move(func_name)) {}
    CallValue(std::string func_name,vector<std::shared_ptr<Expression> > params) : func_name_(std::move(func_name)),params_(
            std::move(params)) {}
    void AddParam(std::shared_ptr<Expression> expr);

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return CALL;}

private:
    std::string func_name_;
    vector<std::shared_ptr<Expression> > params_;
};
```

##### class Variable

该类表示一个pascal变量，如`a count num[3] v[i+1,j]`具体定义如下

```c++
// represent variable in pascal
// e.g. count  num[3+i]  pos[3,4]
// in the example of pos[3,4] the elements of expr_list_ is 3 and 4
// expr_list_ can be empty
class Variable : public Expression {
public:
    explicit Variable(std::string id) : id_(std::move(id)) {}
    Variable(std::string id,vector<std::shared_ptr<Expression> > expr_list) : id_(std::move(id)),expr_list_(
            std::move(expr_list)) {}

    void AddExpr(std::shared_ptr<Expression> expr);

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return VARIABLE;}

private:
    std::string id_;
    vector<std::shared_ptr<Expression> > expr_list_;
};
```

##### class BinaryExpr

该类表示一个二元运算表达式，具体定义如下

```c++
// represent binary expression
// e.g. 3 + 4 + 5    3 * ((6 + 7) - 2)
// in the example of 3 + 4 + 5, lhs_ is 3 + 4 ,rhs_ is 5 and op_ is '+'
class BinaryExpr : public Expression {
public:
    BinaryExpr(int op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs) :
            op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return BINARY;}

private:
    int op_;                 // operator
    std::shared_ptr<Expression> lhs_, rhs_; // two operands
};
```

##### class UnaryExpr

该类表示一个一元运算，如`-3  not true  not abc`等等，具体定义如下

```c++
// represent unary expression
// e.g. -3
// in the above example, op_ is '-' and factor is 3
class UnaryExpr : public Expression {
public:
    UnaryExpr(int op, std::shared_ptr<Expression> factor) : op_(op), factor_(std::move(factor)) {}

    std::string ToString(int level) const override;
    inline ExprType GetType() const override {return UNARY;}

private:
    int op_;
    std::shared_ptr<Expression> factor_;
};
```

### Parser

#### 功能描述

以token为输入，解析Pascal-S中的各种语法结构，如表达式、语句、常量声明、变量声明、子查询定义等。在对符合语法的输入进行解析后生成对应的抽象语法树(AST)，该抽象语法树能完整的表达源码包含的信息，语法分析器生成AST后将交由语义分析模块处理。


#### 接口描述

##### class Parser
| 描述                                               | 接口             | 参数                    | 返回值      | 异常                                                       |
| -------------------------------------------------- | ---------------- | ----------------------- | ----------- | ---------------------------------------------------------- |
| 给定输入文件指针，返回一个针对特定文件的Parser实例 | Parser(FILE *in) | FILE *in : 输入文件指针 | Parser实例  | 无                                                         |
| 分析整个输入文件，得到整颗AST                      | ParseProgram()   | 无                      | Program指针 | 若存在语法错误，则会将所有的错误信息保存在一个成员变量中。 |

#### Parser定义

parser类的定义如下

```c++
class Parser{
public:
    // param:
    //     in is the input file
    explicit Parser(FILE *in);
private:
    int token_,next_token_;                      // current token and next token
  	// if an lex error occur, error number will be stored in this variable
    int lexer_errno_;     
  	// token value
    YYSTYPE tok_value_,next_tok_value_;
    int line_,next_line_;                        // line number of token in the input file
    int column_,next_column_;                      // column number of token in the input file
    std::string text_,next_text_;
  
    vector<std::string> err_msg_;     // error massages
};
```

`Parser`的构造函数接收一个`FILE *`，即输入文件。各个成员变量作用:

- `token_`:当前token
- `lexer_errno_`:若获取下一个token时出现了词法错误，则该变量存着错误号
- `line_`:当前token在源文件的行数
- `column_`:当前token在源文件中的列数
- `text_`:当前token对应的字符串
- `err_msg_`:当出现语法错误时，错误信息均存在这里面

成员中形如`next_***_`的变量为下一个token对应的信息，再做语法分析时有时多看一个token对确定产生式有帮助，故有`next_***_`成员变量。

#### 算法描述（还不完善）

我们采用递归下降的方式来实现语法分析，对于每种语法成分，均编写对应方法进行解析，在这个过程中还会用到一些辅助方法，具体方法描述如下表

|             方法              |                             描述                             |
| :---------------------------: | :----------------------------------------------------------: |
|        ParseProgram()         |                           语法分析                           |
|      ParseProgramHead()       |                          分析程序头                          |
|      ParseProgramBody()       |                         分析程序主体                         |
|   ParseConstDeclarations()    |                         分析常量声明                         |
|    ParseVarDeclarations()     |                         分析变量声明                         |
| ParseSubprogramDeclarations() |                          分析子程序                          |
|     ParseSubprogramHead()     |                         分析子程序头                         |
|     ParseSubprogramBody()     |                        分析子程序主体                        |
|   ParseCompoundStatement()    |                         分析复合语句                         |
|       ParseStatement()        |          解析任意一种`statement`，返回Statement指针          |
|      ParseIfStatement()       |              解析`if`语句，返回`Statement`指针               |
|      ParseForStatement()      |              解析`for`语句，返回`Statement`指针              |
| ParseAssignAndCallStatement() | 解析赋值语句或函数、过程调用语句，返回`Statement`指针。两种语句放在一起解析是因为两种语句均以`TOK_ID`开头 |
|          ParseExpr()          |             解析一个表达式，返回`Expression`指针             |
|      ParseExpr(int prec)      | 解析运算符等级不小于`prec`的表达式，用该函数可以简化表达式解析难度，正确处理结合性问题 |
|        ParsePrimary()         |      解析一个表达式的基本单元，如整数、实数、函数调用等      |

![语法分析流程图](assets/parser_flowchart.jpg)

进行**错误处理**时，将采取如下策略:

- 在较低层的解析方法中碰到语法错误时抛出异常
- 在较高层的解析方法调用底层解析函数时捕获异常，将错误信息存入`err_msg_`
- 继续解析的两种处理策略:
  - 碰到异常后，跳过若干`token`直到碰到想要的`token`
  - 碰到异常后，忽略一些`token`接着解析

如`ParseExpr`和`ParseStatement`在碰到语法错误时直接抛出异常，`ParseCompoundStatement`调用`ParseStatement`来解析一系列语句，若捕获异常，则记录下异常，并不断跳过`token`直到遇到`;`或者可能的语句开头的`token`再接着进行解析。`ParseCompoundStatement`在碰到缺少`begin`的错误时，记录错误并忽略`begin`直接进行statement的解析操作。


---

## 语义分析

### 符号表设计

#### 数据结构说明



#### 函数、方法说明



### ast 处理

---

## 代码生成

### 数据结构说明

### 函数、方法说明

### 算法说明

