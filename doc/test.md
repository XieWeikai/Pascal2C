## 5 测试

- [5 测试](#5-测试)
  - [5.1 词法分析](#51-词法分析)
    - [5.1.1 简单测试套件 (LexerSimpleTest)](#511-简单测试套件-lexersimpletest)
      - [简单测试 (SimpleTest)](#简单测试-simpletest)
      - [Hello World测试 (HelloWorld)](#hello-world测试-helloworld)
    - [5.1.2 注释测试套件 (LexerCommentTest)](#512-注释测试套件-lexercommenttest)
      - [简单注释测试 (SimpleComment)](#简单注释测试-simplecomment)
      - [嵌套注释测试 (NestedComment)](#嵌套注释测试-nestedcomment)
      - [复杂注释测试 (ComplexComment)](#复杂注释测试-complexcomment)
    - [5.1.3 标识符测试套件 (LexerIdentifierTest)](#513-标识符测试套件-lexeridentifiertest)
      - [合法标识符测试 (ValidIdentifier)](#合法标识符测试-valididentifier)
      - [大小写不敏感测试 (CaseInsensitiveTest)](#大小写不敏感测试-caseinsensitivetest)
    - [5.1.4 整数测试套件 (LexerIntegerTest)](#514-整数测试套件-lexerintegertest)
      - [简单整数测试 (SimpleInteger)](#简单整数测试-simpleinteger)
      - [整数长度测试 (IntegerLengthTest)](#整数长度测试-integerlengthtest)
    - [5.1.5 实数测试套件 (LexerRealTest)](#515-实数测试套件-lexerrealtest)
      - [简单实数测试 (SimpleRealTest)](#简单实数测试-simplerealtest)
      - [实数省略测试 (RealOmitNumberTest)](#实数省略测试-realomitnumbertest)
  - [5.2 语法分析](#52-语法分析)
    - [5.2.1 测试环境](#521-测试环境)
    - [5.2.2 测试功能](#522-测试功能)
    - [5.2.3 单元测试用例](#523-单元测试用例)
      - [程序分析测试（TestParseProgram）](#程序分析测试testparseprogram)
      - [程序头部分析测试（TestParseProgramHead）](#程序头部分析测试testparseprogramhead)
      - [程序体分析测试（TestParseProgramBody）](#程序体分析测试testparseprogrambody)
      - [常量声明分析测试（TestParseConstDecl）](#常量声明分析测试testparseconstdecl)
      - [变量声明分析测试（TestParseVarDecl）](#变量声明分析测试testparsevardecl)
      - [子程序声明分析测试（TestParseSubProgramDecl）](#子程序声明分析测试testparsesubprogramdecl)
      - [子程序头部分析测试（TestParseSubProgramHead）](#子程序头部分析测试testparsesubprogramhead)
      - [子程序体分析测试（TestParseSubProgramBody）](#子程序体分析测试testparsesubprogrambody)
      - [标识符列表分析测试（TestParseIdList）](#标识符列表分析测试testparseidlist)
      - [类型分析测试（TestParseType）](#类型分析测试testparsetype)
      - [数组上下界分析测试（TestParsePeriod）](#数组上下界分析测试testparseperiod)
      - [参数分析测试（TestParseParameter）](#参数分析测试testparseparameter)
      - [表达式解析测试(ExprParserTest)](#表达式解析测试exprparsertest)
      - [语句解析测试(StatementParserTest)](#语句解析测试statementparsertest)
      - [procedure](#procedure)
      - [function](#function)
    - [5.2.4 综合分析测试（TotalParserTest）](#524-综合分析测试totalparsertest)
  - [5.3 语义分析](#53-语义分析)
  - [5.4 代码生成](#54-代码生成)
    - [简单程序代码生成测试(CodeGenerationTest)](#简单程序代码生成测试codegenerationtest)
    - [数组测试(ArrayTest)](#数组测试arraytest)
    - [抽象语法树Mock类(ASTMock)](#抽象语法树mock类astmock)
    - [常量声明测试(ConstDeclarationTest)](#常量声明测试constdeclarationtest)
    - [For循环测试(ForStatementTest)](#for循环测试forstatementtest)
    - [函数定义测试(FunctionTest)](#函数定义测试functiontest)
    - [If条件判断语句测试(IfStatementTest)](#if条件判断语句测试ifstatementtest)
    - [Subprogram子程序测试(SubprogramTest)](#subprogram子程序测试subprogramtest)
    - [符号表Mock类](#符号表mock类)
    - [符号表Mock类测试](#符号表mock类测试)
  - [5.5 测试方法](#55-测试方法)
  - [5.6 集成测试](#56-集成测试)


### 5.1 词法分析

词法分析模块使用Google Test进行单元测试，测试用例位于`test/lexer/lexer_test.cc`中. 以下是各个测试套件的简介.

另外, 为了方便交互式测试, 词法分析模块还提供了一个交互式测试程序 `lexer_exe`, 可以从标准输入读入Pascal源代码, 并将词法分析结果输出到标准输出.

#### 5.1.1 简单测试套件 (LexerSimpleTest)

该测试套件用于测试词法分析器在最基本的输入情况下能否正常工作.

##### 简单测试 (SimpleTest)

输入:

```
1 2 3 4
5 6 7 8
```

预期输出:

8个TOK_INTEGER以及对应的行列号和值.

##### Hello World测试 (HelloWorld)

输入:

```
program hello;
begin
    write('Hello, world!\n');
end.
```

预期输出:

每个TOKEN的类型及其对应的行列号. 对于标识符和字符串, 还需要输出其值.

#### 5.1.2 注释测试套件 (LexerCommentTest)

该套件用于测试词法分析器在遇到注释时能否正确跳过注释.

##### 简单注释测试 (SimpleComment)

输入:

```
begin{comment here}begin
```

预期输出:

一个TOK_BEGIN, 一个TOK_BEGIN, 成功跳过注释.

##### 嵌套注释测试 (NestedComment)

输入:

```
begin{comment here{comment here}}begin
```

预期输出:

一个TOK_BEGIN, 一个TOK_BEGIN, 成功跳过注释.

##### 复杂注释测试 (ComplexComment)

输入:

```
{{{{}}}}
begin {begin{end
}}             begin  {begin
}
begin
```

预期输出:

三个TOK_BEGIN, 成功跳过注释.

#### 5.1.3 标识符测试套件 (LexerIdentifierTest)

该套件用于测试词法分析器在遇到各种标识符时能否正确识别.

##### 合法标识符测试 (ValidIdentifier)

输入:

```
begin
    var a, a1, _a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6q_: integer;
    writeln('Valid identifiers');
end.
```

预期输出:

能够正确识别所有合法标识符.

##### 大小写不敏感测试 (CaseInsensitiveTest)

输入:

```
var Identifier, identifier, IDENTIFIER, IdEnTiFiEr: integer;
```

预期输出:

输出四个TOK_ID, 值均为identifier. (Pascal语言中标识符大小写不敏感)

#### 5.1.4 整数测试套件 (LexerIntegerTest)

##### 简单整数测试 (SimpleInteger)

输入:

```
12345, 9494949, 0;
```

预期输出:

识别出其中的三个TOK_INTEGER, 值分别为12345, 9494949, 0.

##### 整数长度测试 (IntegerLengthTest)

输入:

```
2147483647, 2147483648
```

预期输出:

第一个TOK_INTEGER的值为2147483647, 第二个输出为TOK_ERROR, 表示整数溢出.

#### 5.1.5 实数测试套件 (LexerRealTest)

##### 简单实数测试 (SimpleRealTest)

输入:

```
123.456, 123., 123.0, 123.0e10, 123.0e+10, 123.0e-10;
```

预期输出:

识别出其中的六个TOK_REAL, 值分别为123.456, 123., 123.0, 123.0e10, 123.0e+10, 123.0e-10.

##### 实数省略测试 (RealOmitNumberTest)

输入:

```
123.e10, .1e+10;
```

预期输出:

识别出其中的两个TOK_REAL, 值分别为123.0e10, 0.1e+10.

---

### 5.2 语法分析


语法分析模块的测试用例位于`test/parser`目录下。
#### 5.2.1 测试功能

语法分析模块的测试目标是保证语法分析器能够正确的将输入的 token 序列解析成 AST 并且能够正确的处理各种语法错误。具体包括以下几个部分：

* 声明部分解析测试（ProgramParserTest）
  * 程序分析测试（TestParseProgram）
  * 程序头部分析测试（TestParseProgramHead）
  * 程序体分析测试（TestParseProgramBody）
  * 常量声明分析测试（TestParseConstDecl）
  * 变量声明分析测试（TestParseVarDecl）
  * 子程序声明分析测试（TestParseSubProgramDecl）
  * 子程序头部分析测试（TestParseSubProgramHead）
  * 子程序体分析测试（TestParseSubProgramBody）
  * 标识符列表分析测试（TestParseIdList）
  * 类型分析测试（TestParseType）
  * 数组上下界分析测试（TestParsePeriod）
  * 参数分析测试（TestParseParameter）
* 表达式分析测试（ExprParserTest）
  * 表达式因子解析测试（TestParsePrimary）
  * 复杂表达式解析测试（TestParseExpr）
  * 表达式错误检测测试 (TestParserErr)
* 语句分析测试（StatementParserTest）
  * 赋值语句解析测试（TestAssignStatement）
  * if语句分析测试（TestIfStatement）
  * for语句分析测试（TestForStatement）
  * 复合语句分析测试（TestCompoundStatement）
  * 语句解析错误处理测试（TestStatementErr）
* 综合分析测试（TotalParserTest）

各个部分的解析采用手写递归下降的方式来实现，每种语言结构的解析都有对应的方法/函数来完成解析成AST的操作。为了保证代码的正确性，需要对上面提到的各个部分均进行充分的测试。

#### 5.2.2 单元测试用例

对于parser的各个小功能需要分别进行测试以保证整体功能的正确性，在做测试时需要尽可能的保证覆盖率。

##### 程序分析测试（TestParseProgram）


|测试用例|预期结果|测试错误|
|---|---|---|
|program f(a, b); var a, b : integer; begin  end.|Program: <br>-ProgramHead: f<br>--IdList: a, b<br>-ProgramBody: <br>--VarDeclaration: <br>---Type: integer<br>---IdList: a, b<br>--CompoundStatement :0|无|
|program test; var a:integer; begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--VarDeclaration: <br>---Type: integer<br>---IdList: a<br>--CompoundStatement :0|无|
|program test var a: integer; begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--VarDeclaration: <br>---Type: integer<br>---IdList: a<br>--CompoundStatement :0|syntax err:expected ';' before 'var'|
|program test const a = 1; begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--ConstDeclaration: a<br>---1<br>--CompoundStatement :0|syntax err:expected ';' before 'const'|
|program test procedure p;begin end; begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--Subprogram: <br>---SubprogramHead: procedure p<br>---SubprogramBody: <br>----CompoundStatement :0<br>--CompoundStatement :0| syntax err:expected ';' before 'procedure'|
|program test function f : integer;begin end; begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--Subprogram: <br>---SubprogramHead: function f integer<br>---SubprogramBody: <br>----CompoundStatement :0<br>--CompoundStatement :0|syntax err:expected ';' before 'function'|
|program test begin end.|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--CompoundStatement :0|syntax err:expected ';' before 'begin'|
|program test; begin end|Program: <br>-ProgramHead: test<br>-ProgramBody: <br>--CompoundStatement :0|syntax err:expected '.' before end of file|
##### 程序头部分析测试（TestParseProgramHead）


|测试用例|预期结果|测试错误|
|---|---|---|
|program f(a, b);| ProgramHead: f<br>-IdList: a, b|无|
|program f;| ProgramHead: f|无|
|空| ProgramHead:|syntax err:expected 'program' before end of file<br>syntax err:expected 'id' before end of file|
|f|ProgramHead: f|syntax err:expected 'program' before 'id'|
|program|ProgramHead:|syntax err:expected 'id' before end of file|
|(a,b)|ProgramHead: <br>-IdList: a, b|syntax err:expected 'program' before '('<br>syntax err:expected 'id' before '('|
|program f(a, b|ProgramHead: <br>-IdList: a, b|syntax err:expected ')' before end of file|

##### 程序体分析测试（TestParseProgramBody）


|测试用例|预测结果|测试错误|
|---|---|---|
|const a = 1; b = 2; var c, d : integer; procedure p; begin end; function f(a, b : integer) : integer; begin end; begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-ConstDeclaration: b<br>--2<br>-VarDeclaration: <br>--Type: integer<br>--IdList: c, d<br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-Subprogram: <br>--SubprogramHead: function f integer<br>--Parameter: integer<br>---IdList: a, b<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|无|
|const a = 1 b = 2; begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-ConstDeclaration: b<br>--2<br>-CompoundStatement :0| syntax err:expected ';' before 'id'|
|const a = 1 var a : integer; begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-CompoundStatement :0|syntax err:expected ';' before 'var'|
|const a = 1 procedure p; begin end;begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'procedure'|
|const a = 1 function f integer; begin end; begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-Subprogram: <br>--SubprogramHead: function f integer<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'function'|
|const a = 1 begin end|ProgramBody: <br>-ConstDeclaration: a<br>--1<br>-CompoundStatement :0|syntax err:expected ';' before 'begin'|
|var a : integer b : integer ;begin end|ProgramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-VarDeclaration: <br>--Type: integer<br>--IdList: b<br>-CompoundStatement :0|syntax err:expected ';' before 'id'|
|var a : integer procedure p; begin end;begin end|ProgramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'procedure'|
|var a : integer function f integer; begin end; begin end|ProgramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-Subprogram: <br>--SubprogramHead: function f integer<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'function'|
|var a : integer begin end|ProgramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-CompoundStatement :0|syntax err:expected ';' before 'begin'|
|procedure p;begin end function f : integer; begin end; begin end|ProgramBody: <br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-Subprogram: <br>--SubprogramHead: function f integer<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'function'|
|procedure p;begin end procedure p2; begin end; begin end|ProgramBody: <br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-Subprogram: <br>--SubprogramHead: procedure p2<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'procedure'|
|procedure p;begin end begin end|ProgramBody: <br>-Subprogram: <br>--SubprogramHead: procedure p<br>--SubprogramBody: <br>---CompoundStatement :0<br>-CompoundStatement :0|syntax err:expected ';' before 'begin'|

##### 常量声明分析测试（TestParseConstDecl）

|测试用例|预测结果|测试错误|
|---|---|---|
|a=1|ConstDeclaration: a<br>-1|无|
|b=2.0|ConstDeclaration: b<br>-2.0000|无|
|c=-1|ConstDeclaration: c<br>-unary_op:'-'<br>-expr :<br>--1|无|
|d='a'|ConstDeclaration: d<br>-'a'|无|
|空|ConstDeclaration: <br>-0|syntax err:expected 'id' before end of file<br>syntax err:expected '=' before end of file<br>syntax error: parse expression error: no expected token|
|=1|ConstDeclaration: <br>-1|syntax err:expected 'id' before '='|
|a -1|ConstDeclaration: a<br>-unary_op:'-'<br>-expr :<br>--1|syntax err:expected '=' before '-'|
|a 1.1|ConstDeclaration: a<br>-1.1000|syntax err:expected '=' before 'real'|
|a 'a'|ConstDeclaration: a<br>-'a'|syntax err:expected '=' before 'string'|
|a 1|ConstDeclaration: a<br>-1|syntax err:expected '=' before 'integer'|
|a |ConstDeclaration: a<br>-0|syntax err:expected '=' before end of file<br>syntax error: parse expression error: no expected token|

##### 变量声明分析测试（TestParseVarDecl）

|测试用例|预测结果|测试错误|
|---|---|---|
|a, b, c : integer|VarDeclaration: <br>-Type: integer<br>-IdList: a, b, c|无|
|d, e : array [] of integer|VarDeclaration: <br>-Type: array [] of integer<br>-IdList: d, e|无|
|a|VarDeclaration: <br>-Type: unknown<br>-IdList: a|syntax err:expected ':' before end of file<br>syntax err:expected basic type(integer, real, bool, char) or array before end of file|
|a,b,c integer|VarDeclaration: <br>-Type: integer<br>-IdList: a, b, c|syntax err:expected ':' before 'integer_type'|
|a,b,c array [] of integer|VarDeclaration: <br>-Type: array [] of integer<br>-IdList: a, b, c|syntax err:expected ':' before 'array'|
|a,b,c real|VarDeclaration: <br>-Type: real<br>-IdList: a, b, c|syntax err:expected ':' before 'real_type'|
|a,b,c char|VarDeclaration: <br>-Type: char<br>-IdList: a, b, c|syntax err:expected ':' before 'char_type'|
|a,b,c boolean|VarDeclaration: <br>-Type: boolean<br>-IdList: a, b, c|syntax err:expected ':' before 'boolean_type'|

##### 子程序声明分析测试（TestParseSubProgramDecl）

|测试用例|预测结果|测试错误|
|---|---|---|
|function f : integer; begin end|Subprogram: <br>-SubprogramHead: function f integer<br>-SubprogramBody: <br>--CompoundStatement :0|无|
|procedure p; begin end|Subprogram: <br>-SubprogramHead: procedure p<br>-SubprogramBody: <br>--CompoundStatement :0|无|
|procedure p const a = 1;begin end|Subprogram: <br>-SubprogramHead: procedure p<br>-SubprogramBody: <br>--ConstDeclaration: a<br>---1<br>--CompoundStatement :0|Syntax err:expected ';' before 'const'|
|procedure p var a : integer;begin end|Subprogram: <br>-SubprogramHead: procedure p<br>-SubprogramBody: <br>--VarDeclaration: <br>---Type: integer<br>---IdList: a<br>--CompoundStatement :0|Syntax err:expected ';' before 'var'|
|procedure p begin end|Subprogram: <br>-SubprogramHead: procedure p<br>-SubprogramBody: <br>--CompoundStatement :0|Syntax err:expected ';' before 'begin'|

##### 子程序头部分析测试（TestParseSubProgramHead）

|测试用例|预测结果|测试错误|
|---|---|---|
|procedure p(a : integer)|SubprogramHead: procedure p<br>-Parameter: integer<br>--IdList: a|无|
|procedure p|SubprogramHead: procedure p|无|
|procedure|SubprogramHead: procedure|syntax err:expected 'id' before end of file|
|procedure (a : integer)|SubprogramHead: procedure <br>-Parameter: integer<br>--IdList: a|syntax err:expected 'id' before '('|
|procedure p(a : integer; b : integer|SubprogramHead: procedure p<br>-Parameter: integer<br>--IdList: a<br>-Parameter: integer<br>--IdList: b|syntax err:expected ')' before end of file|
|function f(a,b : integer) :integer|SubprogramHead: function f integer<br>-Parameter: integer<br>--IdList: a, b|无|
|function f : integer|SubprogramHead: function f integer|无|
|function|SubprogramHead: function|syntax err:expected 'id' before end of file|
|function (a : integer) : integer|SubprogramHead: function <br>-Parameter: integer<br>--IdList: a|syntax err:expected 'id' before '('|
|function f(a : integer; b : integer|SubprogramHead: function f unknown<br>-Parameter: integer<br>--IdList: a<br>-Parameter: integer<br>--IdList: b|syntax err:expected ')' before end of file<br>syntax err:expected ':' before end of file<br>syntax err:expected basic type(integer, real, bool, char) before end of file|
|function f(a : integer; b : integer : integer|SubprogramHead: function f integer<br>-Parameter: integer<br>--IdList: a<br>-Parameter: integer<br>--IdList: b|syntax err:expected ')' before ':'|
|function f(a : integer, b : integer|SubprogramHead: function f unknown<br>-Parameter: integer<br>--IdList: a|syntax err:expected ')' before ','|
|function f(a : integer) |SubprogramHead: function f unknown<br>-Parameter: integer<br>--IdList: a|syntax err:expected ':' before end of file<br>syntax err:expected basic type(integer, real, bool, char) before end of file|
|function f(a : integer) integer|SubprogramHead: function f integer<br>-Parameter: integer<br>--IdList: a|syntax err:expected ':' before 'integer_type'|
|function f(a : integer) real|SubprogramHead: function f real<br>-Parameter: integer<br>--IdList: a|syntax err:expected ':' before 'real_type'|
|function f(a : integer) boolean|SubprogramHead: function f boolean<br>-Parameter: integer<br>--IdList: a|syntax err:expected ':' before 'boolean_type'|
|function f(a : integer) char|SubprogramHead: function f char<br>-Parameter: integer<br>--IdList: a|syntax err:expected ':' before 'char_type'|
|function f(a : integer) :|SubprogramHead: function f unknown<br>-Parameter: integer<br>--IdList: a|syntax err:expected basic type(integer, real, bool, char) before end of file|

##### 子程序体分析测试（TestParseSubProgramBody）

|测试用例|预测结果|测试错误|
|---|---|---|
|const a = 1; b = 2; var c, d : integer;  begin end|SubprogramBody: <br>-ConstDeclaration: a<br>--1<br>-ConstDeclaration: b<br>--2<br>-VarDeclaration: <br>--Type: integer<br>--IdList: c, d<br>-CompoundStatement :0|无|
|const a = 1 b = 2; begin end|SubprogramBody: <br>-ConstDeclaration: a<br>--1<br>-ConstDeclaration: b<br>--2<br>-CompoundStatement :0|syntax err:expected ';' before 'id'|
|const a = 1 var a : integer; begin end|SubprogramBody: <br>-ConstDeclaration: a<br>--1<br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-CompoundStatement :0|syntax err:expected ';' before 'var'|
|const a = 1 begin end|SubprogramBody: <br>-ConstDeclaration: a<br>--1<br>-CompoundStatement :0|syntax err:expected ';' before 'begin'|
|var a : integer b : integer; begin end|SubprogramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-VarDeclaration: <br>--Type: integer<br>--IdList: b<br>-CompoundStatement :0|syntax err:expected ';' before 'id'|
|var a : integer begin end|SubprogramBody: <br>-VarDeclaration: <br>--Type: integer<br>--IdList: a<br>-CompoundStatement :0|syntax err:expected ';' before 'begin'|

##### 标识符列表分析测试（TestParseIdList）

|测试用例|预测结果|测试错误|
|---|---|---|
|a, b, c|IdList: a, b, c|无|
|a|IdList: a|无|
|空|IdList: |syntax err:expected 'id' before end of file|
|, b|IdList: b|syntax err:expected 'id' before ','|


##### 类型分析测试（TestParseType）

|测试用例|预测结果|测试错误|
|---|---|---|
|integer|Type: integer|无|
|real|Type: real|无|
|boolean|Type: boolean|无|
|char|Type: char|无|
|array [] of integer|Type: array [] of integer|无|
|array [1..10] of integer|Type: array [1..10] of integer|无|
|array [1..10, 20..30] of integer|Type: array [1..10, 20..30] of integer|无|
|空|Type: unknown|syntax err:expected 'id' before end of file|syntax err:expected basic type(integer, real, bool, char) or array before end of file|
|[] of integer|Type: array [] of integer|syntax err:expected basic type(integer, real, bool, char) or array before '['|
|array [1..10 20..30] of integer|Type: array [1..10] of integer|syntax err:expected ']' before 'integer'|
|array [ of integer|Type: array [0..0] of integer|syntax err:expected 'integer' before 'of'<br>syntax err:expected '..' before end of file<br>syntax err:expected 'integer' before end of file<br>syntax err:expected ']' before end of file<br>syntax err:expected 'of' before end of file<br>syntax err:expected basic type(integer, real, bool, char) before end of file|
|array [] integer|Type: array [] of integer|syntax err:expected 'of' before 'integer_type'|
|array [] real|Type: array [] of real|syntax err:expected 'of' before 'real_type'|
|array [] boolean|Type: array [] of boolean|syntax err:expected 'of' before 'boolean_type'|
|array [] char|Type: array [] of char|syntax err:expected 'of' before 'char_type'|
|array [] of|Type: array [] of unknown|syntax err:expected basic type(integer, real, bool, char) before end of file|

##### 数组上下界分析测试（TestParsePeriod）

|测试用例|预测结果|测试错误|
|---|---|---|
|1..10|1..10|无|
|无|0..0|syntax err:expected 'integer' before end of file<br>syntax err:expected '..' before end of file<br>syntax err:expected 'integer' before end of file|
|..10|0..10|syntax err:expected 'integer' before '..'|
|1 10|1..10|syntax err:expected '..' before 'integer'|
|1|1..0|syntax err:expected '..' before end of file<br>syntax err:expected 'integer' before end of file|
|1..|1..0|syntax err:expected 'integer' before end of file|

##### 参数分析测试（TestParseParameter）

|测试用例|预测结果|测试错误|
|---|---|---|
|a, b : integer|Parameter: integer<br>-IdList: a, b|无|
|var a, b : integer|Parameter: var integer<br>-IdList: a, b|无|
|a, b|Parameter: unknown<br>-IdList: a, b|syntax err:expected ':' before end of file<br>syntax err:expected basic type(integer, real, bool, char) before end of file|
|a, b integer|Parameter: integer<br>-IdList: a, b|syntax err:expected ':' before 'integer_type'|
|a, b real|Parameter: real<br>-IdList: a, b|syntax err:expected ':' before 'real_type'|
|a, b boolean|Parameter: boolean<br>-IdList: a, b|syntax err:expected ':' before 'boolean_type'|
|a, b char|Parameter: char<br>-IdList: a, b|syntax err:expected ':' before 'char_type'|
|a, b :|Parameter: unknown<br>-IdList: a, b|syntax err:expected basic type(integer, real, bool, char) before end of file|


##### 表达式解析测试(ExprParserTest)

表达式解析是递归下降解析中最有难度的部分，需要考虑运算符的结合性、优先级、括号等。在`Parser`中较为重要的两个方法为`ParsePrimary`和`ParseExpr`，`ParsePrimary`方法可以解析出一个表达式最基本的构成部分，如`3` `1.234` `-3` `add(3,4)` `true` `false` `abc` `a[10]`等等；`ParseExpr`方法可以解析出一整个表达式.在`ExprParserTest`中，我们对这两个方法进行了测试，同时也测试了碰到错误的表达式时`Parser`的表现。

**TestParsePrimary**
测试样例如下
```pascal
3  1.23  -3  not 4 +4  -4.1234 abcd add(3,4) count[i+1,b+2] say() true false not true
```
|测试用例| 预期结果 |
|:---:|:---:|
|3|解析出一个整数，整数值为3|
|1.23|解析出一个实数，实数值为1.23|
|-3|解析出一个`UnaryExpr`节点，op为'-' `factor`为3对应ast节点|
|not 4|解析出一个`UnaryExpr`节点，op为'not' `factor`为4对应ast节点|
|4    |解析出一个整数，整数值为4|
|+4   |解析出一个`UnaryExpr`节点，op为'+' `factor`为4对应ast节点|
|-4.1234|解析出一个`UnaryExpr`节点，op为'-' `factor`为4.1234对应ast节点|
|abcd|解析出一个`CallOrVar`节点，id为abcd|
|add(3,4)|解析出一个`CallValue`节点，id为add，params包含两个`Expr`节点，分别对应3和4|
|count[i+1,b+2]|解析出一个`Variable`节点，id为count，expr_list包含两个`Expr`节点，分别对应i+1和b+2|
|say()|解析出一个`CallValue`节点，id为say，params为空|
|true|解析出一个`Boolean`节点，value为true|
|false|解析出一个`Boolean`节点，value为false|
|not true|解析出一个`UnaryExpr`节点，op为'not' `factor`为true对应ast节点|

测试结果符合预期，测试通过。

**TestParseExpr**
测试样例如下
```pascal
1 + 2 + 3 + 4;
1 + 2 * 3  ;
(1 + 2) * 3 ;
-(1 + 2) * 3  ;
(-(1 + 2) * 3 <= 5) and (3 > 4) or (4 < 3) ;
(-(1 + 2) * 3 <= 5) or (3 > 4) and (4 < 3) ;
1 + 'a' + 'abc' + a + b ;
true or false and true ;
(true or false) and true ;
true or (false and true) ;
```

上述各个表达式解析的结果应该满足`pascal`中的运算符优先级结合规则，并且括号能改变运算结合顺序。通过`ToString`方法测试结果是否符合预期，最终通过测试。

**TestParserErr**

|测试用例| 预期结果 |
|:---:|:---:|
|a + |在+后parser希望碰到值或id，但没有，故解析出错，抛出异常|
|(a+1|括号不匹配，抛出异常|
|a + * 1 |表达式错误，+后面希望碰到值或id，但碰到了*，故解析错误，抛出异常|
|a[]|数组下标错误希望碰到数值或id，但碰到了]，故解析错误，抛出异常|
|a[1|中括号未正确闭合，抛出异常|

最终测试结果符合预期，测试通过。

##### 语句解析测试(StatementParserTest)

目前支持解析的语句有如下几种:
- compound statement: 复合语句
- assignment statement: 赋值语句
- if statement: if语句
- for statement: for语句
- call statement: 子程序调用语句
- while statement: 循环语句
- exit: 子查询或程序返回语句

对这些语句的解析设计测试程序如下:

**TestAssignStatement**

样例输入:    
```pascal
a := a + 100 ;
abc := 1 + 2 + 3 ;
count[1,2] := 1 + 2 * 3;
res := add(i+1,b+3) * factor() ;
```
|测试用例| 预期结果 |
|:---:|:---:|
|a := a + 100 ;|解析出一条赋值语句，其中var部分对应a，expr部分对应a + 100这个表达式|
|abc := 1 + 2 + 3 ;|解析出一条赋值语句，其中var部分对应abc，expr部分对应1 + 2 + 3这个表达式|
|count[1,2] := 1 + 2 * 3;|解析出一条赋值语句，其中var部分对应count[1,2]，expr部分对应1 + 2 * 3这个表达式|
|res := add(i+1,b+3) * factor() ;|解析出一条赋值语句，其中var部分对应res，expr部分对应add(i+1,b+3) * factor()这个表达式|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestIfStatement**

样例输入:    
```pascal
if a <= limit then
   a := a + 100;
if i <= 10 then
   i := i + 1
else\n"
   i := i - 1;
```
|测试用例| 预期结果 |
|:---:|:---:|
|if a <= limit then a := a + 100;|解析出一条if语句，其中condition部分对应a <= limit，then部分对应a := a + 100这条语句|
|if i <= 10 then i := i + 1 else i := i - 1;|解析出一条if语句，其中condition部分对应i <= 10，then部分对应i := i + 1，else部分对应i := i - 1|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestForStatement**

样例输入:    
```pascal
for i := 1 to 10 do
   count := count + 1;
for i := lower(i) to upper(i) do
**begin**
   sub_program;
   call(1,2,3);
   a := a + 1
end
```
|测试用例| 预期结果 |
|:---:|:---:|
|for i := 1 to 10 do count := count + 1;|解析出一条for语句，其中id部分对应i，from部分对应1，to部分对应10，do部分对应count := count + 1这条语句|
|for i := lower(i) to upper(i) do begin sub_program; call(1,2,3); a := a + 1 end|解析出一条for语句，其中var部分对应i，from部分对应lower(i)，to部分对应upper(i)，do部分对应begin sub_program; call(1,2,3); a := a + 1 end这个复合语句|

上述各个测试样例解析结果都是正确的，因此测试通过。

**TestCompoundStatement**
样例输入
```pascal
begin
   if l < r then
       begin
           mid := (l + r) / 2;
           sort(l,mid);
           sort(mid+1,r)
       end
   else
       write(a,b,c + d)
end;
```
这个测试用例中有两个`CompoundStatement`，一个就是外层的`begin` `end`包围的语句，该`CompoundStatement`由一条`if`语句构成，而`if`语句的`then`部分是一个`CompoundStatement`,该`CompoundStatement`由三条语句构成，分别是`mid := (l + r) / 2;` `sort(l,mid);` `sort(mid+1,r)`.因此用该测试样例不仅可以测试能否正常解析`CompoundStatement`，还可以测试`CompoundStatement`嵌套的情况的解析。

最终解析结果与预期一致，测试通过。

**TestErrorHandle**
在对整个`pascal`源码进行解析时，`subprogram body`和`program body`其实就是一个`CompoundStatement`，因此在解析`subprogram body`和`program body`时，会调用`ParseCompoundStatement`，而在解析语句碰到语法错误时，我们需要能够记录下该错误并能够继续解析，因此我们需要在`ParseCompoundStatement`中进行错误处理，为了测试错误处理效果设计测试样例如下
```pascal
begin
    a : = 1 + 2;
    for := 1 to 10 do
        subprogram;
    if 3 <> 4 then
        a := 3 + 4;

    const a = 'b';
    var c = 'd' 

    my_func (1,2+3;
    c := 1 + 2 + *

    d := (3 * 4 -( 5 - 3))

    if (3 - (4) > c then
        write('hello')

    for c := a to b do begin
        write(a,b,c);
    end
    c[3,4,7 := 4
end.
```
`ParseCompoundStatement`解析上面的语句后应当产生如下错误信息
- "2:7 syntax error: lost ':=' when parsing assign statement",
- "3:9 syntax error: missing id in for statement",
- "8:5 syntax error: declaration is not part of statement",
- "8:13 syntax error: lost ':=' when parsing assign statement",
- "9:5 syntax error: declaration is not part of statement",
- "9:11 syntax error: lost ':=' when parsing assign statement",
- "11:19 syntax error: unclosed parentheses",
- "12:18 syntax error: parse expression error: no expected token",
- "16:5 syntax error: missing ';' at the end of statement",
- "16:21 syntax err:expected ')' before 'then'",
- "19:5 syntax error: missing ';' at the end of statement",
- "22:5 syntax error: missing ';' at the end of statement",
- "22:13 syntax error: unclosed brackets"

经过测试后发现`Parser`可以正确提示上面的错误信息，因此测试通过。


#### 5.2.3 综合分析测试（TotalParserTest）

做完各个部分的单元测试后，需要将各个部分整合为一个完整的parser，然后做集成测试，生成完整的AST，设计一个综合测试用例如下

```pascal
program CompilerTest;

const
  PI = 3.14159;

var
  intArray: array[1..5] of integer;
  realArray: array[2..2] of real;
  i: integer;

function Add(x, y: integer): integer;
begin
  Add := x + y
end;

procedure Swap(var x, y: integer);
var
  temp: integer;
begin
  temp := x;
  x := y;
  y := temp
end;

procedure PrintArrays;
var
  i: integer;
begin
  writeln('Integer array:');
  for i := 1 to 5 do
    writeln('intArray[', i, '] = ', intArray[i]);

  writeln('Real array:');
  for i := -2 to 2 do
    writeln('realArray[', i, '] = ', realArray[i])
end;

{
  This is a comment.
  { This is a nested comment.
    { This is another nested comment. }
  }
}
begin
  for i := 1 to 5 do
    intArray[i] := i * 2;

  realArray[-2] := 0.5;
  realArray[-1] := 1.5;
  realArray[0] := 2.5;
  realArray[1] := 3.5;
  realArray[2] := 4.5;

  writeln('Before swap:');
  PrintArrays;

  if Add(intArray[2], intArray[4]) = 13 then
  begin
    writeln('Swapping intArray[2] and intArray[4]');
    Swap(intArray[2], intArray[4])
  end;

  writeln('After swap:');
  PrintArrays;

  writeln('PI = ', PI)
end.
```

测试生成的 AST 如下：

```
Program: 
-ProgramHead: compilertest
-ProgramBody: 
--ConstDeclaration: pi
------3.1416
--VarDeclaration: 
---Type: array [1..5] of integer
---IdList: intarray
--VarDeclaration: 
---Type: array [2..2] of real
---IdList: realarray
--VarDeclaration: 
---Type: integer
---IdList: i
--Subprogram: 
---SubprogramHead: function add integer
----Parameter: integer
-----IdList: x, y
---SubprogramBody: 
----CompoundStatement :1
----statement 1:
-----AssignStatement :
-----Variable:
------variable:add
-----Expr :
------binary_op:'+'
------lhs :
-------CallOrVar: x
------rhs :
-------CallOrVar: y
--Subprogram: 
---SubprogramHead: procedure swap
----Parameter: var integer
-----IdList: x, y
---SubprogramBody: 
----VarDeclaration: 
-----Type: integer
-----IdList: temp
----CompoundStatement :3
----statement 1:
-----AssignStatement :
-----Variable:
------variable:temp
-----Expr :
------CallOrVar: x
----statement 2:
-----AssignStatement :
-----Variable:
------variable:x
-----Expr :
------CallOrVar: y
----statement 3:
-----AssignStatement :
-----Variable:
------variable:y
-----Expr :
------CallOrVar: temp
--Subprogram: 
---SubprogramHead: procedure printarrays
---SubprogramBody: 
----VarDeclaration: 
-----Type: integer
-----IdList: i
----CompoundStatement :4
----statement 1:
-----CallStatement :
-----name:writeln
-----expr 1:
------string: Integer array:
----statement 2:
-----ForStatement:
-----id: i
-----from:
------1
-----to:
------5
-----do:
------CallStatement :
------name:writeln
------expr 1:
-------string: intArray[
------expr 2:
-------CallOrVar: i
------expr 3:
-------string: ] = 
------expr 4:
-------variable:intarray
-------index 1:
--------CallOrVar: i
----statement 3:
-----CallStatement :
-----name:writeln
-----expr 1:
------string: Real array:
----statement 4:
-----ForStatement:
-----id: i
-----from:
------unary_op:'-'
------expr :
-------2
-----to:
------2
-----do:
------CallStatement :
------name:writeln
------expr 1:
-------string: realArray[
------expr 2:
-------CallOrVar: i
------expr 3:
-------string: ] = 
------expr 4:
-------variable:realarray
-------index 1:
--------CallOrVar: i
--CompoundStatement :12
--statement 1:
---ForStatement:
---id: i
---from:
----1
---to:
----5
---do:
----AssignStatement :
----Variable:
-----variable:intarray
-----index 1:
------CallOrVar: i
----Expr :
-----binary_op:'*'
-----lhs :
------CallOrVar: i
-----rhs :
------2
--statement 2:
---AssignStatement :
---Variable:
----variable:realarray
----index 1:
-----unary_op:'-'
-----expr :
------2
---Expr :
--------0.5000
--statement 3:
---AssignStatement :
---Variable:
----variable:realarray
----index 1:
-----unary_op:'-'
-----expr :
------1
---Expr :
--------1.5000
--statement 4:
---AssignStatement :
---Variable:
----variable:realarray
----index 1:
-----0
---Expr :
--------2.5000
--statement 5:
---AssignStatement :
---Variable:
----variable:realarray
----index 1:
-----1
---Expr :
--------3.5000
--statement 6:
---AssignStatement :
---Variable:
----variable:realarray
----index 1:
-----2
---Expr :
--------4.5000
--statement 7:
---CallStatement :
---name:writeln
---expr 1:
----string: Before swap:
--statement 8:
---CallStatement :
---name:printarrays
--statement 9:
---IfStatement :
---condition:
----binary_op:'='
----lhs :
-----function:add
-----expr 1:
------variable:intarray
------index 1:
-------2
-----expr 2:
------variable:intarray
------index 1:
-------4
----rhs :
-----13
---if_part:
----CompoundStatement :2
----statement 1:
-----CallStatement :
-----name:writeln
-----expr 1:
------string: Swapping intArray[2] and intArray[4]
----statement 2:
-----CallStatement :
-----name:swap
-----expr 1:
------variable:intarray
------index 1:
-------2
-----expr 2:
------variable:intarray
------index 1:
-------4
--statement 10:
---CallStatement :
---name:writeln
---expr 1:
----string: After swap:
--statement 11:
---CallStatement :
---name:printarrays
--statement 12:
---CallStatement :
---name:writeln
---expr 1:
----string: PI = 
---expr 2:
----CallOrVar: pi";
```

---

### 5.3 语义分析

在保证输入AST有效的前提下，可将输入代码转换为AST观察输出进行测试。

总体测试输入pascal代码如下

```pascal
program SATest;

const
  PI = 3.14159;{常量定义}

var
  intVar: integer;{普通变量定义}
  boolVar: boolean;
  realVar: real;
  intArray: array[1..5] of integer;{数组定义}
  realArray: array[2..10] of real;
  intArray2D:array[0..5,1..5] of integer;{多维数组定义}
  intArray3D:array[0..5,0..5,1..5] of integer;
  realArray3D:array[0..5,0..5,1..5] of real;

function Add(x, y: integer): integer;{函数定义}
const 
    PIINADD = 3;{函数内常量定义}
var 
    intVarInAdd:integer;{函数内变量定义}
    intArrayInAdd: array[1..5] of integer;{函数内数组定义}
    realArrayInAdd: array[2..2] of real;
    intArray2DInAdd:array[0..5,1..5] of integer;{函数内多维数组定义}
    intArray2DInAdd2:array[2..5,2..5] of integer;
    intArray3DInAdd:array[0..5,0..5,1..5] of integer;
    realArray3DInAdd:array[0..5,0..5,1..5] of real;
begin
  {正常通过}
  intVarInAdd := PIINADD;{函数内调用内部变量，函数内调用内部常量}
  realVar := PI;{函数内调用外部变量，函数内调用外部常量}
  realVar := intVar;{int赋值real}
  intArray[3] := PIINADD;{一维数组赋值}
  intArray2DInAdd[1,1] := PIINADD;{多维数组赋值}
  intArray3DInAdd[1] := intArray2DInAdd;
  Add := x + y;{数组返回值赋值，数组参数引用}

  {errors}
  intVar := realVar;{real赋值int}
  intArray2DInAdd := intArray3DInAdd;{多维数组赋值:2d=3d}
  intArray2DInAdd2 := intArray2DInAdd;{多维数组赋值:数组大小不匹配}
  intArray2DInAdd[1,1,1] := PIINADD{多维数组引用:2d数组当3d用}
end;

function AddVar(var x: integer):integer;{传引用函数定义}
begin
    AddVar(x){函数递归调用}
end;
function AddVoid : integer;{空返回值函数定义}
begin
    AddVoid:=1
end;
function AddVar(var x,y:integer):integer;{函数重载}
begin
    AddVar(x)
end;
begin
    {正常通过}
    intArray[1] := intVar * 2 + 1 - intArray2D[1,1];{加减乘语句}
    realVar := 5/2;{除语句}
    intVar := 5 div 2;
    intVar := 5 mod 2;{模语句}
    realVar := 5 mod 2; 


    if not ((intVar=1) and (realVar<3) and (realVar>5) or (AddVoid<>0) and (intArray2D[1,2]>=0) and (intArray2D[2,3]<=0)){>,<,<>,=,>=,<=,and,or,not} 
    then 
        intVar := AddVoid + 1{空参数列表函数调用}   
    else 
        realArray[-2] := 0.5;{数组赋值常量}

    for intVar := 1 to 5 do intVar := -1;{‘-’}

    read(intVar);{read函数}
    readln(intVar);{readln函数}
    write('hello world');{write函数}
    writeln('hello world');{writeln函数}
    AddVar(intVar,intVar);{函数调用及重载调用}
    AddVar(intVar);


    {errors}
    intVar := 4/2;{'/'做除号返回为非int类}
    PI := 3.14;{常量赋值}
    intVar := not_defined_int;{使用未定义变量}
    intVar:=false;{赋值类型错误}
    intVar:=boolVar;
    boolVar:=AddVoid;

    if intVar then intVar := -1 else intVar := -1;{if语句条件不为bool}

    for boolVar:=false to true do intVar := -1;{for语句非int}

    AddVar(intVar,intVar,intVar);{未定义函数调用}
    not_defined_func(intVar);
    AddVar(AddVoid,1){传引用错误} 
end.
```

代码分为正常通过部分和报错部分，其中注释说明了测试条目。

语义分析结果输出如下：

```cmd
ERROR in Line(38) Cloumn(3):Assign Statement failure(type not match): left:INT right:REAL
ERROR in Line(39) Cloumn(3):Assign Statement failure(type not match): left:INT[0..5,1..5] right:INT[0..5,0..5,1..5]
ERROR in Line(40) Cloumn(3):Assign Statement failure(type not match): left:INT[2..5,2..5] right:INT[0..5,1..5]
ERROR in Line(41) Cloumn(3):Assign Statement failure(left not found)
ERROR in Line(83) Cloumn(5):Assign Statement failure(type not match): left:INT right:REAL
ERROR in Line(84) Cloumn(5):Assign Statement failure(left is const)
ERROR in Line(85) Cloumn(15):not_defined_int not found
ERROR in Line(85) Cloumn(5):Assign Statement failure(right not found)
ERROR in Line(86) Cloumn(5):Assign Statement failure(type not match): left:INT right:BOOL
ERROR in Line(87) Cloumn(5):Assign Statement failure(type not match): left:INT right:BOOL
ERROR in Line(88) Cloumn(5):Assign Statement failure(type not match): left:BOOL right:INT
ERROR in Line(90) Cloumn(5):If Statement failure(condition error:received type INT)
ERROR in Line(92) Cloumn(5):For Statement failure(type error in id from to):id:BOOL from:BOOL to:BOOL
ERROR in Line(94) Cloumn(5):Call Statement failure(Found but not match,asking for const func ERROR addvar[var INT ][var INT ][var INT ])
ERROR in Line(95) Cloumn(5):Call Statement failure(Not found,asking for const func ERROR not_defined_func[var INT ])
ERROR in Line(96) Cloumn(5):Call Statement failure(Found but paramenter not match,asking for const func ERROR addvar[const INT ][const INT ])
```

---

### 5.4 代码生成

为了确保代码生成模块工作正常，使用了 Google Test 编写了一系列单元测试。这些单元测试涵盖了从基本数据类型和表达式到复杂控制结构的各种情况，确保了代码生成器和 AST 适配器的正确性。通过运行这些单元测试，可以确保该模块的实现正确地处理了源代码，并生成了预期的 C 代码。

此外, 为了避免其他模块的变动影响单元测试对本模块测试的效果, 该模块对其他模块没有
依赖. 如果用到其他模块的功能, 使用Mock类实现.

此外，由于代码生成是整个编译器的最后一个环节，代码生成的许多节点的功能直接借助集成测试进行了测试。集成测试可以确保各个模块之间的协同工作，验证整个编译器的工作流程。这样，我们可以在更大的范围内确认代码生成模块在实际场景中的表现和可靠性。

通过结合单元测试和集成测试，我们对代码生成模块进行了全面的测试，确保了编译器的正确性和鲁棒性。这些测试帮助我们发现和修复潜在的问题，提高了代码的质量和可维护性。

#### 简单程序代码生成测试(CodeGenerationTest)
这个测试代码包括两个部分：一个名为 SimpleProgramAST 的辅助函数，用于创建一个简单程序的 AST 结构；以及一个名为 CodeGeneratorTest 的测试用例，用于验证代码生成器在处理此简单程序时是否能正确地生成 C 代码。

辅助函数 SimpleProgramAST 构建了一个简单的 Pascal 程序的 AST 结构。这个程序声明了两个整数变量 x 和 y，并为它们分别分配了 2 + 3 和 x - 1 的计算结果。在构建 AST 结构时，我们使用了各种 AST 节点类，如 VarDeclaration、BinaryOperation、Assignment 等。

CodeGeneratorTest 测试用例首先通过 SimpleProgramAST 函数创建了一个简单程序的 AST。然后，我们创建了一个符号表的模拟对象 s_table，并使用它实例化了一个 CodeGenerator 对象。接下来，我们调用 Interpret() 方法，将程序的 AST 根节点传递给代码生成器。代码生成器遍历程序的 AST 结构，生成相应的 C 代码。最后，我们通过 GetCCode() 方法获取生成的 C 代码，并将其与预期的 C 代码进行比较。如果生成的 C 代码与预期相符，测试用例将被认为是通过的。

这个测试用例验证了代码生成器在处理简单的 Pascal 程序时能够正确地生成 C 代码。通过这种方式，我们可以确保代码生成模块在实际应用中能够处理各种类型的 Pascal 程序，并生成正确的 C 代码。

```cpp
std::shared_ptr<Program> SimpleProgramAST() {
    // vector<std::shared_ptr<ASTNode>> declarations;
    auto declarations =
        std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();

    // VarDecl part
    const auto type_int = std::make_shared<Type>(
        make_shared<Token>(TokenType::RESERVED, "integer"));
    const auto var_x =
        std::make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "x"));
    const auto var_decl_x =
        std::make_shared<VarDeclaration>(std::move(var_x), std::move(type_int));

    const auto var_y =
        std::make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "y"));
    const auto var_decl_y =
        std::make_shared<VarDeclaration>(std::move(var_y), std::move(type_int));

    declarations->push_back(std::move(var_decl_x));
    declarations->push_back(std::move(var_decl_y));

    // Compound statement part
    auto children = std::make_shared<std::vector<std::shared_ptr<ASTNode>>>();

    // +
    const auto plus_op =
        std::make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
    // -
    const auto minus_op =
        std::make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "-"));

    // 2 + 3
    const auto num_2 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "2"));
    const auto num_3 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3"));
    const auto bin_plus_op = std::make_shared<BinaryOperation>(
        std::move(num_2), std::move(plus_op), std::move(num_3));

    // x - 1
    const auto num_1 =
        std::make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
    const auto bin_minus_op = std::make_shared<BinaryOperation>(
        std::move(var_x), std::move(minus_op), std::move(num_1));

    const auto assign_x =
        std::make_shared<Assignment>(std::move(var_x), std::move(bin_plus_op));
    const auto assign_y =
        std::make_shared<Assignment>(std::move(var_y), std::move(bin_minus_op));

    // Construct compound
    children->push_back(std::move(assign_x));
    children->push_back(std::move(assign_y));
    const auto compound = std::make_shared<Compound>(std::move(*children));

    const auto declaration =
        std::make_shared<Declaration>(std::move(*declarations));

    auto block =
        std::make_shared<Block>(std::move(declaration), std::move(compound));
    auto program = std::make_shared<Program>("Simple", std::move(block));

    return program;
}

TEST(GeneratorTest, CodeGeneratorTest) {
    string source_code = R"(program Simple;
var
    x, y: integer;
begin
    x := 2 + 3;
    y := x - 1;
end.
)";

    auto program = SimpleProgramAST();

    auto s_table = make_shared<SymbolTableMock>();
    auto code_generator = std::make_shared<CodeGenerator>(s_table);
    code_generator->Interpret(program);
    auto generated_ccode = code_generator->GetCCode();

    string expected_c_code = "#include <stdio.h>\n"
                             "#include <stdlib.h>\n"
                             "#include <stdbool.h>\n"
                             "\n"
                             "int x;\n"
                             "int y;\n"
                             "// Simple\n"
                             "int main(int argc, char* argv[]) {\n"
                             "    x = (2 + 3);\n"
                             "    y = (x - 1);\n"
                             "    return 0;\n"
                             "}\n";
    std::cout << generated_ccode << endl;
    EXPECT_EQ(generated_ccode, expected_c_code);
}
```

测试输出
```cpp
[----------] 2 tests from GeneratorTest
[ RUN      ] GeneratorTest.CodeGeneratorTest
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int x;
int y;
// Simple
int main(int argc, char* argv[]) {
    x = (2 + 3);
    y = (x - 1);
    return 0;
}

[       OK ] GeneratorTest.CodeGeneratorTest (0 ms)
```

#### 数组测试(ArrayTest)

这部分主要包括数组声明测试. 数组的访问测试在`FunctionTest`, `SubprogramTest`和集成测试中进行了详细的测试.

该测试是针对数组部分的代码生成的测试，定义了一个 `ArrayTest` 类，继承自 `Google` `Test` 的 ::`testing`::`Test` 类。测试类包含了一些保护成员变量，用于存储测试相关的数据和 `AST` 节点。在测试类的 `SetUp`() 方法中，我们初始化了这些变量，构建了一个简单的数组声明的 `AST` 结构。

`ArrayTest` 类中包含如下成员变量：

`expected_array_declaration_statement_`：预期的 `C` 代码数组声明语句。
`name_`：数组的名称。
`array_declaration_`：一个 `ArrayDeclaration` 类型的智能指针，表示数组声明。
`array_`：一个 `Array` 类型的智能指针，表示数组。
`array_var_`：一个 `Var` 类型的智能指针，表示数组变量。
`array_type_`：一个 `ArrayType` 类型的智能指针，表示数组的类型。
`type_`：一个 `Type` 类型的智能指针，表示数组元素的类型。
`ArrayCreation` 测试用例首先创建了一个符号表的模拟对象 `s_table`，然后使用这个模拟对象实例化一个 `CodeGenerator` 对象。接下来，我们调用 `Interpret`() 方法，将数组声明的根节点传递给代码生成器。代码生成器遍历数组声明的 `AST` 结构，生成相应的 `C` 代码。最后，我们通过 `GetCCode`() 方法获取生成的 `C` 代码，并将其与预期的 `C` 代码数组声明语句进行比较。如果生成的 `C` 代码与预期相符，测试用例将被认为是通过的。

这个测试用例验证了代码生成器在处理数组声明时能够正确地生成 `C` 代码。通过这种方式，我们可以确保数组部分的代码生成工作正常，并进一步提高整个代码生成模块的可靠性。

```cpp
class ArrayTest : public ::testing::Test {
  protected:
    void SetUp() override {
        name_ = "arr";
        type_ = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        std::vector<pair<int, int>> bounds = {{1, 100}};
        array_type_ = make_shared<ArrayType>(type_, bounds);
        array_var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, name_));
        array_ = make_shared<Array>(array_var_, bounds);
        array_declaration_ = make_shared<ArrayDeclaration>(array_, array_type_);
        // Expected C array declaration statement
        expected_array_declaration_statement_ = "int arr[100];\n";

        ASSERT_NE(array_var_, nullptr);
        ASSERT_NE(array_, nullptr);
        ASSERT_NE(type_, nullptr);
        ASSERT_NE(array_type_, nullptr);
        ASSERT_NE(array_declaration_, nullptr);
        ASSERT_EQ(name_, "arr");
    }
    void TearDown() override {}

    string expected_array_declaration_statement_;
    string name_;
    shared_ptr<ArrayDeclaration> array_declaration_;
    shared_ptr<Array> array_;
    shared_ptr<Var> array_var_;
    shared_ptr<ArrayType> array_type_;
    shared_ptr<Type> type_;
};

TEST_F(ArrayTest, ArrayCreation) {
    auto array_declaration_root = array_declaration_;
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator code_generator(s_table);
    code_generator.Interpret(array_declaration_root);
    auto generated_ccode = code_generator.GetCCode();
    cout << generated_ccode << endl;
    ASSERT_EQ(generated_ccode, expected_array_declaration_statement_);
}
```

测试输出
```bash
[----------] 1 test from ArrayTest
[ RUN      ] ArrayTest.ArrayCreation
int arr[100];

[       OK ] ArrayTest.ArrayCreation (0 ms)
[----------] 1 test from ArrayTest (0 ms total)
```

#### 抽象语法树Mock类(ASTMock)
这些 `Mock` 类是用于测试的辅助类，用于模拟抽象语法树（`AST`）中的各种节点类型。通过使用这些 `Mock` 类，我们可以在测试中轻松地创建 `AST` 节点对象，并控制它们的行为，以便在不实际执行实现代码的情况下测试其他代码。这些 `Mock` 类分别继承自各种 `AST` 节点类，并使用 `Google` `Mock` 库提供的宏定义方法。

以下是各个 `Mock` 类的简要介绍：

1. `MockCompound`：模拟复合语句节点，具有方法 `GetChildren`，该方法返回包含子语句的向量。
1. `MockBlock`：模拟块节点，具有方法 `GetDeclarations` 和 `GetCompoundStatement`，分别返回声明向量和复合语句节点。
1. `MockProgram`：模拟程序节点，具有方法 `GetName` 和 `GetBlock`，分别返回程序名称和块节点。
1. `MockVar`：模拟变量节点，具有方法 `GetValue`，该方法返回变量名。
1. `MockType`：模拟类型节点，具有方法 `GetType`，该方法返回类型的 `TokenType`。
1. `MockVarDecl`：模拟变量声明节点，具有方法 `GetVarNode` 和 `GetTypeNode`，分别返回变量节点和类型节点。
1. `MockAssign`：模拟赋值节点，具有方法 `GetLeft` 和 `GetRight`，分别返回赋值操作的左侧和右侧节点。
1. `MockTerm`、`MockFactor` 和 `MockExpr`：模拟术语、因子和表达式节点。这些类没有额外的方法，仅用作占位符。
1. `MockNum`：模拟数字节点，具有方法 `GetValue`，该方法返回数字值。
1. `MockBinOp`：模拟二元操作节点，具有方法 `GetLeft`、`GetOper` 和 `GetRight`，分别返回二元操作的左侧节点、操作符 `Token` 和右侧节点。
1. `MockNoOp`：模拟无操作节点，该类没有额外的方法，仅用作占位符。
1. 这些 `Mock` 类可以用于编写针对代码生成器、语法分析器等其他模块的单元测试。通过使用这些类，我们可以轻松地构建和控制 `AST` 节点，以便在不执行实际实现代码的情况下测试其他代码的功能。

```cpp
class MockCompound : public ASTNode {
  public:
    MOCK_METHOD(const vector<std::shared_ptr<ASTNode>> &, GetChildren, (),
                (const));
};
class MockBlock : public ASTNode {
  public:
    MOCK_METHOD(const vector<std::shared_ptr<ASTNode>> &, GetDeclarations, (),
                (const));
    MOCK_METHOD(const std::shared_ptr<Compound> &, GetCompoundStatement, (),
                (const));
};
class MockProgram : public ASTNode {
  public:
    MOCK_METHOD(string, GetName, (), (const));
    MOCK_METHOD(const std::shared_ptr<MockBlock> &, GetBlock, (), (const));
};

class MockVar : public ASTNode {
  public:
    MOCK_METHOD(const string, GetValue, (), (const));
};
class MockType : public ASTNode {
  public:
    MOCK_METHOD(const TokenType, GetType, (), (const));
};
class MockVarDecl : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<MockVar> &, GetVarNode, (), (const));
    MOCK_METHOD(const std::shared_ptr<Type> &, GetTypeNode, (), (const));
};
class MockAssign : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetLeft, (), (const));
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetRight, (), (const));
};
class MockTerm : public ASTNode {
  public:
};
class MockFactor : public ASTNode {};
class MockExpr : public ASTNode {};
class MockNum : public ASTNode {
  public:
    MOCK_METHOD(int, GetValue, (), (const));
};
class MockBinOp : public ASTNode {
  public:
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetLeft, ());
    MOCK_METHOD(const std::shared_ptr<Token> &, GetOper, ());
    MOCK_METHOD(const std::shared_ptr<ASTNode> &, GetRight, ());
};
class MockNoOp : public ASTNode {};
```

#### 常量声明测试(ConstDeclarationTest)

这个测试用例主要针对常量声明（ConstDeclaration）类进行测试。测试包括以下几个方面：

使用 ConstDeclarationTest 测试固件类，该类继承自 Google Test 提供的 ::testing::Test 类。在 SetUp 方法中，我们创建了一个字符类型的 ConstType 对象、一个名为 K_A 的 Var 对象，并将它们用于创建 ConstDeclaration 对象。同时，我们定义了一个预期的 C 代码字符串 expected_ccode_，用于与生成的 C 代码进行比较。

在 TearDown 方法中，我们不需要执行任何特殊操作，因此保留为空。

使用 TEST_F 宏定义一个名为 ConstDeclarationCreation 的测试用例。首先，我们检查 const_type_ 和 var_ 对象的属性是否与预期相符。

接下来，我们创建一个 SymbolTableMock 对象，并使用它初始化 CodeGenerator 类。我们将 const_decl_ 对象传递给 code_generator 的 Interpret 方法，以便生成相应的 C 代码。我们使用 GetCCode 方法获取生成的 C 代码，并将其输出到控制台。

最后，我们使用 ASSERT_EQ 断言检查生成的 C 代码是否与预期的 C 代码字符串 expected_ccode_ 相匹配。

通过这个测试用例，我们可以验证 ConstDeclaration 类及其与 CodeGenerator 类的交互是否正常工作，以便正确生成常量声明的 C 代码。
```cpp
class ConstDeclarationTest : public ::testing::Test {
  protected:
    void SetUp() override {
        const_type_ = make_shared<ConstType>(
            make_shared<Token>(TokenType::RESERVED, "char"));
        var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "K_A"));
        const_decl_ = make_shared<ConstDeclaration>(var_, const_type_);
    }
    void TearDown() override {}

    string expected_ccode_ = "const char K_A;\n";
    shared_ptr<ConstDeclaration> const_decl_;
    shared_ptr<ConstType> const_type_;
    shared_ptr<Var> var_;
};

TEST_F(ConstDeclarationTest, ConstDeclarationCreation) {
    EXPECT_EQ(const_type_->GetType(), "char");
    EXPECT_EQ(var_->GetName(), "K_A");
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator code_generator(s_table);
    code_generator.Interpret(const_decl_);
    auto ccode = code_generator.GetCCode();
    cout << ccode;
    ASSERT_EQ(ccode, expected_ccode_);
}
```

**测试输出**
```cpp
[----------] 1 test from ConstDeclarationTest
[ RUN      ] ConstDeclarationTest.ConstDeclarationCreation
const char K_A;
[       OK ] ConstDeclarationTest.ConstDeclarationCreation (0 ms)
[----------] 1 test from ConstDeclarationTest (0 ms total)
```

#### For循环测试(ForStatementTest)
这个测试用例主要针对 `ForStatement` 类进行测试。测试的目标是确保正确生成对应的 `C` 代码。具体的测试步骤如下：

1. 使用 `ForStatementTest` 测试固件类，该类继承自 `Google` `Test` 提供的 ::`testing`::`Test` 类。在 `SetUp` 方法中，我们定义了一个预期的 `C` 代码字符串 `expected_ccode_`，用于与生成的 `C` 代码进行比较。

1. 创建一个名为 `a` 的 `Var` 对象和一个名为 `i` 的 `Var` 对象（用作循环变量）。我们还创建一个表示加法运算符的 `Oper` 对象，一个表示起始值 1 的 `Num` 对象，以及一个表示结束值 10 的 `Num` 对象。

1. 接下来，我们创建一个 `BinaryOperation` 对象，用于表示 `a` 和 `i` 之间的加法操作。我们将这个操作作为 `Assignment` 对象的右侧操作数，将 `a` 作为左侧操作数，以表示对 `a` 进行赋值的操作。

1. 创建一个 `Compound` 对象，将 `Assignment` 对象作为其子节点。然后，我们创建一个 `ForStatement` 对象，将循环变量 `i`、起始值 `start_`、结束值 `end_` 和循环体 `body_` 传递给它。

1. 使用 `TEST_F` 宏定义一个名为 `ForStatementCreation` 的测试用例。首先，我们创建一个 `SymbolTableMock` 对象，并使用它初始化 `CodeGenerator` 类。将 `for_statement_` 对象传递给 `CodeGenerator` 的 `Interpret` 方法，以生成相应的 `C` 代码。

1. 使用 `GetCCode` 方法获取生成的 `C` 代码，并将其输出到控制台。最后，我们使用 `EXPECT_EQ` 断言检查生成的 `C` 代码是否与预期的 `C` 代码字符串 `expected_ccode_` 相匹配。

通过这个测试用例，我们可以验证 `ForStatement` 类及其与 `CodeGenerator` 类的交互是否正常工作，以便正确生成 `for` 循环语句的 `C` 代码。

```cpp
class ForStatementTest : public ::testing::Test {
  protected:
    void SetUp() override {
        expected_ccode_ = R"(for (i = 1; i <= 10; i++) {
    a = (a + i);
}
)";

        auto a_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "a"));
        auto for_var_ =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "i"));
        auto plus_ =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
        auto start_ =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
        auto end_ =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "10"));

        auto bin_op_ = make_shared<BinaryOperation>(a_, plus_, for_var_);
        auto assign_ = make_shared<Assignment>(a_, bin_op_);
        auto body_ =
            make_shared<Compound>(std::vector<shared_ptr<ASTNode>>{assign_});

        for_statement_ =
            make_shared<ForStatement>(for_var_, start_, end_, body_);
    }

    string expected_ccode_;
    shared_ptr<ForStatement> for_statement_;
};

TEST_F(ForStatementTest, ForStatementCreation) {
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator cg(s_table);
    cg.Interpret(for_statement_);
    cout << cg.GetCCode();
    EXPECT_EQ(cg.GetCCode(), expected_ccode_);
}
```

**测试输出**
```bash
[----------] 1 test from ForStatementTest
[ RUN      ] ForStatementTest.ForStatementCreation
for (i = 1; i <= 10; i++) {
    a = (a + i);
}
[       OK ] ForStatementTest.ForStatementCreation (0 ms)
[----------] 1 test from ForStatementTest (0 ms total)
```

#### 函数定义测试(FunctionTest)

这个测试用例主要针对 `Function` 类进行测试。测试的目标是确保正确生成对应的 `C` 代码。具体的测试步骤如下：

1. 使用 `FunctionTest` 测试固件类，该类继承自 `Google` `Test` 提供的 ::`testing`::`Test` 类。在 `SetUp` 方法中，我们定义了一个预期的 `C` 代码字符串 `expected_ccode_`，用于与生成的 `C` 代码进行比较。

1. 设置函数名称、参数、以及函数体。我们为函数创建两个参数：一个是传值参数，另一个是传引用参数。接着，构建一个函数体，包括一些数组访问、二元操作、赋值操作等。最后，我们使用这些信息创建一个 `Function` 对象。

1. 使用 `TEST_F` 宏定义一个名为 `FunctionDeclarationTest` 的测试用例。首先，我们创建一个 `SymbolTableMock` 对象，并使用它初始化 `CodeGenerator` 类。

1. 为模拟符号表设置一些预期调用和返回值。这包括用于确定变量是否是引用类型和是否是返回值变量的函数调用。

1. 将 `function_` 对象传递给 `CodeGenerator` 的 `Interpret` 方法，以生成相应的 `C` 代码。

1. 使用 `GetCCode` 方法获取生成的 `C` 代码，并将其输出到控制台。最后，我们使用 `ASSERT_EQ` 断言检查生成的 `C` 代码是否与预期的 `C` 代码字符串 `expected_ccode_` 相匹配。

通过这个测试用例，我们可以验证 `Function` 类及其与 `CodeGenerator` 类的交互是否正常工作，以便正确生成函数声明和定义的 `C` 代码。

```cpp
class FunctionTest : public ::testing::Test {
  protected:
    void SetUp() override { // Set name of function
        function_name_ = "test_function";

        // Set args for function
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"));
        auto type_by_reference =
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "char"));
        auto arg_by_reference =
            make_shared<Argument>(var_by_reference, type_by_reference, true);
        var_by_value = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_value"));
        auto array_bounds_by_value =
            std::vector<pair<int, int>>{make_pair(3, 5), make_pair(7, 16)};
        auto array_by_value =
            make_shared<Array>(var_by_value, array_bounds_by_value);
        auto type_by_value =
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "int"));
        auto array_type_by_value =
            make_shared<ArrayType>(type_by_value, array_bounds_by_value);
        auto arg_by_value =
            make_shared<Argument>(array_by_value, array_type_by_value, false);
        std::vector<shared_ptr<Argument>> args = {
            arg_by_value,
            arg_by_reference,
        };

        // Construct body block for function
        auto compound = make_shared<Compound>();
        // *by_reference = (by_value[5][9] + by_value[0][19]);
        // by_value[5][9]
        auto array_access_by_value_5_9 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "9"))});
        // by_value[0][19]
        auto array_access_by_value_0_19 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "0")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "19"))});
        // BinaryOperation
        auto bin_op = make_shared<BinaryOperation>(
            array_access_by_value_5_9,
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+")),
            array_access_by_value_0_19);
        // Assignment
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"), true);
        auto assign_by_reference =
            make_shared<Assignment>(var_by_reference, bin_op);

        // by_value[3][4] = 5;
        auto array_access_by_value_3_4 = make_shared<ArrayAccess>(
            array_by_value,
            std::vector<shared_ptr<ASTNode>>{
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3")),
                make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "4"))});
        auto assign_by_value = make_shared<Assignment>(
            array_access_by_value_3_4,
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")));

        // test_function = by_value[3][4];
        auto var_return = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, function_name_), false,
            true);
        auto assign_return =
            make_shared<Assignment>(var_return, array_access_by_value_3_4);
        compound->AddChild(assign_by_reference);
        compound->AddChild(assign_by_value);
        compound->AddChild(assign_return);
        auto body = make_shared<Block>(compound);
        function_ = make_shared<Function>(
            function_name_,
            make_shared<Type>(make_shared<Token>(TokenType::RESERVED, "char")),
            args, body);
    }

    string expected_ccode_ =
        R"(char test_function(int[3][10] by_value, /* Is Reference */char *by_reference) {
    char ret_test_function;/* Auto Generated */
    *by_reference = (by_value[5 - 3][9 - 7] + by_value[0 - 3][19 - 7]);
    by_value[3 - 3][4 - 7] = 5;
    ret_test_function = by_value[3 - 3][4 - 7];
    return ret_test_function;/* Auto Generated */
}
)";

    shared_ptr<Var> var_by_reference;
    shared_ptr<Var> var_by_value;
    string function_name_;
    shared_ptr<Function> function_;
};

TEST_F(FunctionTest, FunctionDeclarationTest) {
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator cg(s_table);
    EXPECT_CALL(*s_table, IsReference(function_name_))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReference(var_by_reference->GetName()))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReference(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReturnVar(function_name_))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReturnVar(var_by_reference->GetName()))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*s_table, IsReturnVar(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    cg.Interpret(function_);
    auto code = cg.GetCCode();
    cout << code << endl;
    ASSERT_EQ(code, expected_ccode_);
}
```

**测试结果**
```cpp
[----------] 1 test from FunctionTest
[ RUN      ] FunctionTest.FunctionDeclarationTest
char test_function(int[3][10] by_value, /* Is Reference */char *by_reference) {
    char ret_test_function;/* Auto Generated */
    *by_reference = (by_value[5 - 3][9 - 7] + by_value[0 - 3][19 - 7]);
    by_value[3 - 3][4 - 7] = 5;
    ret_test_function = by_value[3 - 3][4 - 7];
    return ret_test_function;/* Auto Generated */
}

[       OK ] FunctionTest.FunctionDeclarationTest (0 ms)
[----------] 1 test from FunctionTest (0 ms total)
```

#### If条件判断语句测试(IfStatementTest)
这个测试用例主要针对 `IfStatement` 类进行测试。测试的目标是确保正确生成对应的 `C` 代码。具体的测试步骤如下：

1. 使用 `IfStatementTest` 测试固件类，该类继承自 `Google` `Test` 提供的 ::`testing`::`Test` 类。在 `SetUp` 方法中，我们定义了一个预期的 `C` 代码字符串 `expected_ccode_`，用于与生成的 `C` 代码进行比较。

1. 设置条件、`if` 分支和 `else` 分支。我们创建一个条件表达式 `a` == `b`，然后创建两个复合语句作为 `if` 分支和 `else` 分支。在 `if` 分支中，我们添加一个赋值语句 `a` = 1 + 2；在 `else` 分支中，我们添加另一个赋值语句 `a` = 2 * 3。最后，我们使用这些信息创建一个 `IfStatement` 对象。

1. 使用 `TEST_F` 宏定义一个名为 `IfStatementCreation` 的测试用例。首先，我们创建一个 `SymbolTableMock` 对象，并使用它初始化 `CodeGenerator` 类。

1. 将 `if_statement_` 对象传递给 `CodeGenerator` 的 `Interpret` 方法，以生成相应的 `C` 代码。

1. 使用 `GetCCode` 方法获取生成的 `C` 代码，并将其输出到控制台。最后，我们使用 `EXPECT_EQ` 断言检查生成的 `C` 代码是否与预期的 `C` 代码字符串 `expected_ccode_` 相匹配。

通过这个测试用例，我们可以验证 `IfStatement` 类及其与 `CodeGenerator` 类的交互是否正常工作，以便正确生成 `if` 语句的 `C` 代码。

```cpp
class IfStatementTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // condition: a == b
        auto var_a =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "a"));
        auto var_b =
            make_shared<Var>(make_shared<Token>(TokenType::IDENTIFIER, "b"));
        auto eq =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "="));
        auto a_eq_b = make_shared<BinaryOperation>(var_a, eq, var_b);
        auto condition = a_eq_b;
        auto then_branch = make_shared<Compound>();
        auto else_branch = make_shared<Compound>();

        // a = 1 + 2;
        auto num_1 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1"));
        auto num_2 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "2"));
        auto plus =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "+"));
        auto then_binop = make_shared<BinaryOperation>(num_1, plus, num_2);
        auto then_assign = make_shared<Assignment>(var_a, then_binop);
        then_branch->AddChild(then_assign);

        // a = 2 * 3;
        auto num_3 =
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "3"));
        auto mul =
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "*"));
        auto else_binop = make_shared<BinaryOperation>(num_2, mul, num_3);
        auto else_assign = make_shared<Assignment>(var_a, else_binop);
        else_branch->AddChild(else_assign);

        if_statement_ =
            make_shared<IfStatement>(condition, then_branch, else_branch);
    }

    string expected_ccode_ = R"(if ((a == b)) {
    a = (1 + 2);
} else {
    a = (2 * 3);
}
)";
    shared_ptr<IfStatement> if_statement_;
};

TEST_F(IfStatementTest, IfStatementCreation) {
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator cg(s_table);
    cg.Interpret(if_statement_);
    auto ccode = cg.GetCCode();
    cout << ccode;
    EXPECT_EQ(ccode, expected_ccode_);
}
```

**测试输出**
```bash
[----------] 1 test from IfStatementTest
[ RUN      ] IfStatementTest.IfStatementCreation
if ((a == b)) {
    a = (1 + 2);
} else {
    a = (2 * 3);
}
[       OK ] IfStatementTest.IfStatementCreation (0 ms)
[----------] 1 test from IfStatementTest (0 ms total)
```

#### Subprogram子程序测试(SubprogramTest)
这个测试用例主要针对 `Subprogram` 类进行测试。测试的目标是确保正确生成对应的 `C` 代码。具体的测试步骤如下：

1. 使用 `SubprogramTest` 测试固件类，该类继承自 `Google` `Test` 提供的 ::`testing`::`Test` 类。在 `SetUp` 方法中，我们定义了一个预期的 `C` 代码字符串 `expected_ccode_`，用于与生成的 `C` 代码进行比较。

1. 设置子程序名称、参数和子程序体。我们创建一个名为 `test_subprogram` 的子程序，其参数包括一个按引用传递的整数变量 `by_reference` 和一个整数数组变量 `by_value`。然后，我们创建一个复合语句作为子程序体。在子程序体中，我们添加两个赋值语句：*`by_reference` = `by_value` * 1 和 `by_value`[3] = 5。最后，我们使用这些信息创建一个 `Subprogram` 对象。

1. 使用 `TEST_F` 宏定义一个名为 `SubprogramDeclaration` 的测试用例。首先，我们创建一个 `SymbolTableMock` 对象，并使用它初始化 `CodeGenerator` 类。

1. 将 `subprogram_` 对象传递给 `CodeGenerator` 的 `Interpret` 方法，以生成相应的 `C` 代码。

1. 使用 `GetCCode` 方法获取生成的 `C` 代码，并将其输出到控制台。最后，我们使用 `ASSERT_EQ` 断言检查生成的 `C` 代码是否与预期的 `C` 代码字符串 `expected_ccode_` 相匹配。

通过这个测试用例，我们可以验证 `Subprogram` 类及其与 `CodeGenerator` 类的交互是否正常工作，以便正确生成子程序的 `C` 代码。

```cpp
class SubprogramTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // Set name of subprogram
        subprogram_name_ = "test_subprogram";

        // Set args for subprogram
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"));
        auto type_by_reference = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        auto arg_by_reference =
            make_shared<Argument>(var_by_reference, type_by_reference, true);

        var_by_value = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_value"));
        auto array_bounds_by_value =
            std::vector<pair<int, int>>{make_pair(3, 5)};
        auto array_by_value =
            make_shared<Array>(var_by_value, array_bounds_by_value);
        auto type_by_value = make_shared<Type>(
            make_shared<Token>(TokenType::RESERVED, "integer"));
        auto array_type_by_value =
            make_shared<ArrayType>(type_by_value, array_bounds_by_value);
        auto arg_by_value =
            make_shared<Argument>(array_by_value, array_type_by_value, false);
        std::vector<shared_ptr<Argument>> args = {arg_by_reference,
                                                  arg_by_value};

        // Construct body block for subprogram
        auto compound = make_shared<Compound>();
        // *by_reference = by_value * 1;
        auto bin_op = make_shared<BinaryOperation>(
            var_by_value,
            make_shared<Oper>(make_shared<Token>(TokenType::OPERATOR, "*")),
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "1")));
        var_by_reference = make_shared<Var>(
            make_shared<Token>(TokenType::IDENTIFIER, "by_reference"), true);
        auto assign_by_reference =
            make_shared<Assignment>(var_by_reference, bin_op);

        // by_value[3] = 5;
        auto array_access_by_value_3 = make_shared<ArrayAccess>(
            array_by_value, std::vector<shared_ptr<ASTNode>>{make_shared<Num>(
                                make_shared<Token>(TokenType::NUMBER, "3"))});
        auto assign_by_value = make_shared<Assignment>(
            array_access_by_value_3,
            make_shared<Num>(make_shared<Token>(TokenType::NUMBER, "5")));
        compound->AddChild(assign_by_reference);
        compound->AddChild(assign_by_value);
        auto body = make_shared<Block>(compound);
        subprogram_ = make_shared<Subprogram>(subprogram_name_, args, body);
    }

    shared_ptr<Var> var_by_reference;
    shared_ptr<Var> var_by_value;
    string expected_ccode_ =
        R"(void test_subprogram(/* Is Reference */int *by_reference, int[3] by_value) {
    *by_reference = (by_value * 1);
    by_value[3 - 3] = 5;
}
)";
    string subprogram_name_;
    shared_ptr<Subprogram> subprogram_;
};

TEST_F(SubprogramTest, SubprogramDeclaration) {
    // Mock symbol table used for IsReference Lookup
    auto s_table = make_shared<SymbolTableMock>();
    CodeGenerator cg(s_table);
    EXPECT_CALL(*s_table, IsReference(var_by_reference->GetName()))
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*s_table, IsReference(var_by_value->GetName()))
        .WillRepeatedly(Return(false));
    cg.Interpret(subprogram_);
    auto code = cg.GetCCode();
    cout << code << endl;
    ASSERT_EQ(code, expected_ccode_);
}
```

**测试输出**
```bash
[----------] 1 test from SubprogramTest
[ RUN      ] SubprogramTest.SubprogramDeclaration
void test_subprogram(/* Is Reference */int *by_reference, int[3] by_value) {
    *by_reference = (by_value * 1);
    by_value[3 - 3] = 5;
}

[       OK ] SubprogramTest.SubprogramDeclaration (0 ms)
[----------] 1 test from SubprogramTest (0 ms total)
```

#### 符号表Mock类
`SymbolScopeMock` 和 `SymbolTableMock` 类是用于测试的模拟类，它们分别实现了 `ISymbolScope` 和 `ISymbolTable` 接口。这两个模拟类允许我们在测试中方便地替换实际的符号表实现，以便在不实际访问符号表的情况下测试与符号表交互的代码。

`SymbolScopeMock` 类的 `API` 如下：

1. `Lookup` 方法：接受一个字符串类型的变量名，返回一个 `shared_ptr`<`SymbolItem`> 类型的指针。这个方法在给定的作用域中查找一个符号项。
1. `IsReference` 方法：接受一个字符串类型的变量名，返回一个布尔值。这个方法检查给定变量名是否表示一个引用类型。
1. `IsReturnVar` 方法：接受一个字符串类型的变量名，返回一个布尔值。这个方法检查给定变量名是否表示一个返回变量。

`SymbolTableMock` 类的 `API` 如下：

1. `IsReference` 方法：与 `SymbolScopeMock` 类的 `IsReference` 方法类似，但在整个符号表中查找。
1. `IsReturnVar` 方法：与 `SymbolScopeMock` 类的 `IsReturnVar` 方法类似，但在整个符号表中查找。
1. `SetCurrentScope` 方法：接受一个字符串类型的作用域名。这个方法设置当前的作用域。
1. `GetCurrentScope` 方法：无参数，返回一个字符串类型的值。这个方法返回当前的作用域名。
1. `Lookup` 方法：与 `SymbolScopeMock` 类的 `Lookup` 方法类似，但在整个符号表中查找。
这两个模拟类在测试中非常有用，因为它们允许我们模拟符号表的行为，而无需实际实现复杂的符号表逻辑。使用 `Google` `Mock`，我们可以定义这些方法的预期行为，以便在测试中控制它们的返回值和调用次数。这使得我们可以专注于测试其他部分的代码逻辑，而不用担心符号表的实现细节。

```cpp
class SymbolScopeMock : public ISymbolScope {
  public:
    MOCK_METHOD(const shared_ptr<SymbolItem>, Lookup, (const string &name),
                (const, override));
    MOCK_METHOD(bool, IsReference, (const string &name), (const, override));
    MOCK_METHOD(bool, IsReturnVar, (const string &name), (const, override));
};

class SymbolTableMock : public ISymbolTable {
  public:
    MOCK_METHOD(bool, IsReference, (const string &name), (const, override));
    MOCK_METHOD(bool, IsReturnVar, (const string &name), (const, override));
    MOCK_METHOD(void, SetCurrentScope, (const string &scope_name), (override));
    MOCK_METHOD(const string, GetCurrentScope, (), (override));
    MOCK_METHOD(const shared_ptr<SymbolItem>, Lookup, (const string &name),
                (const, override));
};
```

#### 符号表Mock类测试
本测试名为 `MockSymbolTableTest`，位于 `SymbolTableTest` 测试套件中。该测试主要用于验证 `SymbolTableMock` 和 `SymbolScopeMock` 模拟类的功能和行为。测试中创建了两个模拟对象，一个是 `symbol_table_mock`（模拟 `SymbolTable`），另一个是 `symbol_scope_mock`（模拟 `SymbolScope`）。

在这个测试中，我们定义了以下期望行为：

1. 当在 `symbol_table_mock` 中调用 `IsReference` 方法并传入变量名 `x` 时，预期返回 `false`。
1. 当在 `symbol_table_mock` 中调用 `Lookup` 方法并传入变量名 `x` 时，预期返回一个表示非引用类型的 `SymbolItem` 对象。
1. 当在 `symbol_scope_mock` 中调用 `Lookup` 方法并传入变量名 `x` 时，预期返回一个表示非引用类型的 `SymbolItem` 对象。

在定义了预期行为后，我们按照以下步骤进行测试：

1. 调用 `symbol_scope_mock` 的 `Lookup` 方法，并传入变量名 `x`。
1. 调用 `symbol_table_mock` 的 `Lookup` 方法，并传入变量名 `x`。将返回的 `SymbolItem` 对象存储在 `item_x` 变量中。
1. 使用 `EXPECT_FALSE` 宏，检查 `symbol_table_mock` 的 `IsReference` 方法返回值是否为 `false`。
1. 使用 `EXPECT_FALSE` 宏，检查 `item_x` 的 `IsReference` 方法返回值是否为 `false`。
通过这个测试，我们可以验证 `SymbolTableMock` 和 `SymbolScopeMock` 的预期行为是否正确，以确保它们在其他测试中可靠地模拟符号表的行为。

```cpp
TEST(SymbolTableTest, MockSymbolTableTest) {
    auto x = std::make_shared<Var>("x");

    auto symbol_table_mock = std::make_shared<SymbolTableMock>();
    auto symbol_scope_mock = std::make_shared<SymbolScopeMock>();

    EXPECT_CALL(*symbol_table_mock, IsReference(x->GetName()))
        .WillOnce(Return(false));
    EXPECT_CALL(*symbol_table_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", false, false)));
    EXPECT_CALL(*symbol_scope_mock, Lookup("x"))
        .WillOnce(Return(std::make_shared<SymbolItem>("x", false, false)));

    symbol_scope_mock->Lookup(x->GetName());
    auto item_x = symbol_table_mock->Lookup(x->GetName());
    EXPECT_FALSE(symbol_table_mock->IsReference(x->GetName()));
    EXPECT_FALSE(item_x->IsReference());
}
```

**测试输出**
```bash
[----------] 1 test from SymbolTableTest
[ RUN      ] SymbolTableTest.MockSymbolTableTest
[       OK ] SymbolTableTest.MockSymbolTableTest (0 ms)
[----------] 1 test from SymbolTableTest (0 ms total)
```

为了方便测试，在前三部分测试完毕后，直接以pascal源代码作为输入，生成AST来测试代码生成模块，测试样例如下

```pascal
program merge_sort;

const
    n = 10;

var
    mas: array [1..n] of integer;
    i: integer;

procedure MergeSort(a, c: integer);
var
    x, j, i, n1, n2: integer;
    rez: array[1..1000] of integer;
begin
    if c <= a then 
        exit 
    else 
    begin
        x := (a + c) div 2;
        MergeSort(a, x);
        MergeSort(x + 1, c);
        n1 := a;
        n2 := x + 1;
        for i := a to c do 
        begin
            if (n1 < x + 1) and ((n2 > c) or (mas[n1] < mas[n2])) then
            begin
                rez[i] := mas[n1];
                inc(n1);
            end 
            else 
            begin
                rez[i] := mas[n2];
                inc(n2);
            end;
        end;
        for j := a to c do
            mas[j] := rez[j];
    end; 
end;

begin
    for i := 1 to n do
        mas[i] := random(20);
    writeln(mas);
    MergeSort(1, n);
    writeln(mas);
end.
```

### 5.5 测试方法

本项目各个模块内部的方法测试使用google test进行测试，测试代码均位于`test`目录下，测试内容如前文所述。在各模块分别完成测试后，编写主程序将各模块组合起来进行集成测试，集成测试阶段我们将编写一些简单的pascal程序作为输入，将产生的c语言代码进行编译运行，检查输出是否符合预期。

### 5.6 集成测试

在分别完成各个模块的测试后，将各个模块组合起来，直接测试能否将Pascal转化为C语言并在讲C语言编译为可执行文件后表现出和pascal一样的结果。测试样例如下：

**归并排序**
```pascal
{ mergesort.pas }
program merge_sort;

var
    n : integer;
    mas: array [1..1000] of integer;
    i: integer;

procedure inc(var a: integer);
begin
    a := a + 1;
end;

procedure My_MergeSort(a, c: integer);
var
    x, j, i, n1, n2: integer;
    rez: array[1..1000] of integer;
begin
    if c <= a then
        exit
    else
    begin
        x := (a + c) div 2;
        My_MergeSort(a, x);
        My_MergeSort(x + 1, c);
        n1 := a;
        n2 := x + 1;
        for i := a to c do
        begin
            if (n1 < x + 1) and ((n2 > c) or (mas[n1] < mas[n2])) then
            begin
                rez[i] := mas[n1];
                inc(n1);
            end
            else
            begin
                rez[i] := mas[n2];
                inc(n2);
            end;
        end;
        for j := a to c do
            mas[j] := rez[j];
    end;
end;

begin
    read(n);
    for i := 1 to n do
        read(mas[i]);

    My_MergeSort(1, n);
    for i := 1 to n do
        write(mas[i], ' ');
    writeln;
end.
```

**快速排序**
```pascal
{ quicksort.pas }
program QuickSort;
var
  arr : array[1..100] of integer;
  len, i : integer;

procedure swap(var a, b: integer);
var
  temp: integer;
begin
  temp := a;
  a := b;
  b := temp;
end;

procedure QuickSort(l, r: integer);
var
  i, j, p: integer;
begin
  if l < r then
  begin
    i := l;
    j := r;
    p := arr[(l + r) div 2];

    while i <= j do
    begin
      while arr[i] < p do
        i := i + 1;
      while arr[j] > p do
        j := j - 1;

      if i <= j then
      begin
        swap(arr[i], arr[j]);
        i := i + 1;
        j := j - 1;
      end;
    end;

    if l < j then
      QuickSort(l, j);
    if i < r then
      QuickSort(i, r);
  end;
end;

begin
  read(len);
  for i := 1 to len do
    read(arr[i]);

  QuickSort(1, len);

  for i := 1 to len do
    write(arr[i], ' ');
  writeln;
end.

```

**求最大公因数**
```pascal
{ gcd.pas }
program gcd;

var x,y : integer;

function gcd(a,b : integer) : integer;
begin
    if b = 0 then gcd := a
    else gcd := gcd(b, a mod b)
end;

begin
    read(x,y);
    write('gcd(',x,',',y,') = ');
    writeln(gcd(x,y))
end.
```

**求阶乘**
```pascal
{ factorial.pas }
program fac;

var n: integer;

function factorial(n: integer): integer;
begin
  if n = 0 then
    factorial := 1
  else
    factorial := n * factorial(n - 1)
end;

function factorial2(n: integer): integer;
var
  i: integer;
begin
    factorial2 := 1;
    for i := 1 to n do
        factorial2 := factorial2 * i
end;

function factorial3(n: integer): integer;
begin
  if n = 0 then
  begin
    factorial3 := 1;
    exit
  end;
  factorial3 := n * factorial3(n - 1)
end;

begin
    read(n);
    writeln(factorial(n));
    writeln(factorial2(n));
    writeln(factorial3(n))
end.
```

**特性测试**
```pascal
{ features.pas }
program TestPascal;

const
    PI = 3.14159;

var
    i, j: integer;
    r: real;
    a: array [1..5] of integer;

procedure printArray(hi: integer);
var
    i: integer;
begin
    for i := 1 to hi do
        write(a[i], ' ');
    writeln;
end;

function add(x, y: integer): integer;
begin
    add := x + y;
end;

procedure swap(var x, y: integer);
var
    temp: integer;
begin
    temp := x;
    x := y;
    y := temp;
end;

begin
    writeln('Hello, \'', PI, '\'!');

    i := 5;
    j := 10;
    r := i / j;
    writeln('Real division: ', r);

    r := i div j;
    writeln('Integer division: ', r);

    writeln('Complex expression: ', ((i + j) * i - j) mod i);

    for i := 1 to 5 do
        a[i] := i * i;

    printArray(3);

    writeln('Function add: ', add(i, j));

    writeln('Before swap: ', i, ' ', j);
    swap(i, j);
    writeln('After swap: ', i, ' ', j);

    while i > 0 do begin
        writeln('Countdown: ', i);
        i := i - 1;
    end;
end.
```

**复杂表达式测试**
```pascal
{ complex_exprssion.pas }
program ComplexExpressionTest;

var
    i: integer;
    r, s, t: real;

function add(x: real; y: integer): real;
begin
    add := x + y;
end;

function square(x: real): real;
begin
    square := x * x;
end;

function increment: integer;
begin
    increment := 1 + 1;
end;

begin
    i := 5;
    r := 2.5;

    s := i + square(r) * add(r, increment) - (i mod increment) / square(r);

    writeln('Result: ', s);
end.
```

**总体测试**
```pascal
{ general.pas }
program test;

const PI = 3.14;
var tmp : integer;

function value() : integer;
begin
    value := 1;
end;

procedure expression;
const scalar = 2;
var
    a : real;
    b : real;
    c : integer;
    flag : boolean;
begin
    c := 9 div 2 * 3 mod 4;
    a := (PI + value * 2 - (4 - 2)) * 2;
    flag := (a > 0) and (a < 10) or (a = 0) or (a = 10) or (a <= 0) and (a >= 10) or (a <> 8) and true and not false;
    b := scalar * 3 / 4 + 2 * (1.6 - 2.5);
    writeln('a = ', a);
    writeln('b = ', b);
    writeln('c = ', c);
    writeln('flag = ', flag);
end;

function var_test(var a : integer; b : integer) : integer;
begin
    a := b;
end;

function exit_test(a : integer) : boolean;
begin
    if a > 0 then
    begin
        writeln('a > 0');
        exit_test := true;
        exit;
    end
    else writeln('a <= 0');

    exit_test := false;
end;

procedure while_test(n : integer);
var i : integer;
begin
    writeln('while_test');
    i := 0;
    while i <= n do
    begin
        writeln('i = ', i);
        i := i + 1;
    end;
end;

function for_test(n : integer) : integer;
var sum : integer;
    i : integer;
begin
    sum := 0;
    for i := 1 to n do
        sum := sum + i;
    for_test := sum;
end;

begin
    expression;
    writeln('expression done.');
    var_test(tmp, 20 + value);
    writeln('var_tmp done. tmp = ', tmp);
    writeln('exit_test exit_test(tmp) = ', exit_test(tmp));
    writeln('exit_test exit_test(-tmp) = ', exit_test(-tmp));
    writeln('exit_test done.');
    while_test(10);
    writeln('while_test done.');
    writeln('for_test for_test(10) = ', for_test(10));
    writeln('for_test done.');
end.
```
**错误检测**
```pascal
{ error.pas }
program error;

const b;

var a : integer;

procedure test(var a : integer);
begin
	a := a + 1
end;

begin
	a := 3 + 4
	a := 3.5 + 3.2 * 3;
	test(a);
	test(3);
end.
```
经过测试，`error.pas`在经过`passcal2c`后能报出存在的语法错误和语义错误，其余各个pascal程序在转化为C后，编译出来的可执行文件均能正常运行并得到正确的结果。
