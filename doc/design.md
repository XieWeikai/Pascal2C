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


#### 数据结构定义

##### class Program
  AST 树根节点，存有指向 `ProgramHead`、`ProgramBody`的指针

        // Program -> ProgramHead; ProgramBody.
        class Program
        {
        public:
            Program(shared_ptr<ProgramHead> program_head, shared_ptr<ProgramBody> program_body)
                : program_head_(std::move(program_head)), program_body_(std::move(program_body)) {}

            const string ToString(const int &level) const;

        private:
            shared_ptr<ProgramHead> program_head_; //Program head
            shared_ptr<ProgramBody> program_body_; //Program body
        };



### Parser

#### 功能描述

以token为输入，解析Pascal-S中的各种语法结构，如表达式、语句、常量声明、变量声明、子查询定义等。在对符合语法的输入进行解析后生成对应的抽象语法树(AST)，该抽象语法树能完整的表达源码包含的信息，语法分析器生成AST后将交由语义分析模块处理。


#### 接口描述

##### class Parser
  | 描述                                                          | 接口             | 参数                    | 返回值 | 异常              |
  | ------------------------------------------------------------- | ---------------- | ----------------------- | ------ | ----------------- |
  | 输入程序文件，调用词法分析接口对程序进行语法分析，输出 AST 树 | Parser(FILE *in) | FILE *in : 输入文件指针 | 无     | parser::SyntaxErr |

#### 数据结构定义



#### 算法描述

| 系统事件名称                             | 用例           | 参数说明                              |
| ---------------------------------------- | -------------- | ------------------------------------- |
| SyntaxAnalysis(tokens)                   | 语法分析       | 1. tokens:vector\<Token>              |
| ParseProgramHead(tokens, pos)            | 分析程序头     | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseProgramBody(tokens, pos)()          | 分析程序主体   | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseConstDeclarations(tokens, pos)      | 分析常量声明   | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseVarDeclarations(tokens, pos)        | 分析变量声明   | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseSubprogramDeclarations(tokens, pos) | 分析子程序     | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseSubprogramHead(tokens, pos)         | 分析子程序头   | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseSubprogramBody(tokens, pos)         | 分析子程序主体 | 1. tokens:vector\<Token>; 2. pos:int; |
| ParseCompoundStatements(tokens, pos)     | 分析复合语句   | 1. tokens:vector\<Token>; 2. pos:int; |

![语法分析流程图](assets/parser_flowchart.jpg)



#### 算法描述

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

